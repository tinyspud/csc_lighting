fn = 'vis';

% mx = 255;
mx = 176;
mn = 33;

lox = 210;
hix = 247;
loy = 1;
hiy = 56;

hgt = hix-lox + 1;
wdh = (hiy-loy + 1) / 8;

chars = zeros(hix-lox + 1, hiy-loy + 1, mx-mn + 1);
wids = zeros(length(chars(1, 1, :)), 2);
widthofchars = length(chars(1, :, 1));
hgts = wids; % the top starting row and bottom ending row

fontsize = 20;

defthresh = 128;
redthresh = defthresh;
bluthresh = defthresh;
grnthresh = defthresh;
fid_header = fopen([fn,'.h'], 'w');
fid_source = fopen([fn,'.c'], 'w');

% Write the top of the header for the header file
fwrite(fid_header, ['/* Created by C Chock - Beaumont - autogenerated with MATLAB code\r\n * create_chars.m */',char(13),char(10)]);
fwrite(fid_header, ['#ifndef __HELV_CHARS',char(13),char(10)]);
fwrite(fid_header, ['#define __HELV_CHARS',char(13),char(10)]);
fwrite(fid_header, ['#include "hal_stdtypes.h"',char(13),char(10)]);
fwrite(fid_header, ['int __HELV_b_cur_wid; ',char(13), char(10)]);
fwrite(fid_header, ['int __HELV_b_cur_lstart; ',char(13), char(10)]);
fwrite(fid_header, ['int __HELV_b_cur_height; ',char(13), char(10)]);
fwrite(fid_header, ['int __HELV_b_cur_top; ',char(13), char(10)]);
fwrite(fid_header, ['int __HELV_b_cur_bottom; ',char(13), char(10)]);
fwrite(fid_header, [char(13), char(10)]);
fwrite(fid_header, ['#define __HELV_HEIGHT  ',num2str(hgt),char(13),char(10)]);
fwrite(fid_header, ['#define __HELV_WID  ',num2str(wdh),char(13),char(10)]);

% Write the header of the c file
fwrite(fid_source, ['/* Created by C Chock - Beaumont - autogenerated with MATLAB code',char(13),char(10),' * create_chars.m */',char(13),char(10)]);
fwrite(fid_source, ['#include "',fn,'.h"',char(13),char(10)]);
fwrite(fid_source, ['uint8* get_char_graphic_pointer(char char_to_place){',char(13),char(10),'    uint8* rtnval = 0;',char(13),char(10)]);
fwrite(fid_source, ['    switch(char_to_place){',char(13),char(10)]);
% ui_display_set_pixel_foreground()

for i = mn:mx
    thisidx = i - mn + 1;
    
    hf = figure('color','white','units','pixel','position',[500,500,200,250]);
    image(ones([40,50,3]));
    axis off;
    if char(i) == '\' || char(i) == '{' || char(i) == '}' || char(i) == '_' || char(i) == '^'
        text('units','pixels','position',[-20 0],'fontsize',fontsize,'fontname','helvetica','string',['\',char(i)]);
    else
        text('units','pixels','position',[-20 0],'fontsize',fontsize,'fontname','helvetica','string',char(i));
    end
    % text('units','pixels','position',[0 0],'fontsize',10,'fontname','helvetica','string','a');
    tim = getframe(hf);
    chars(:, :, thisidx) = ((tim.cdata(lox:hix, loy:hiy, 1) <= redthresh) |...
        (tim.cdata(lox:hix, loy:hiy, 2) <= bluthresh) | ...
        (tim.cdata(lox:hix, loy:hiy, 3) <= grnthresh));
    close all;
    % write a row
    fwrite(fid_header, ['/* CHAR: ',char(i),' */',char(13),char(10)]);
    % Get the width of the character and define it
    fwrite(fid_header, ['/* Width of  ',char(i),' */',char(13),char(10)]);
    
    % Find the left most start
    wid = 0;
    lstart = 0;
    for widfind = 1:widthofchars
        % find the width
        if ~isempty(find(chars(:, widfind, thisidx), 1))
            lstart = widfind;
            break;
        end
    end
    
    for widfind = widthofchars:-1:1
        % find the width
        if ~isempty(find(chars(:, widfind, thisidx), 1))
            wid = widfind;
            break;
        end
    end
    
    wids(thisidx, :) = [wid - lstart, lstart];
    
    if wid == -1 || lstart == -1
        continue;
    end
    
    % Write the header
    fwrite(fid_header, ['/* Column of leftmost pixel */',char(13),char(10)]);
    fwrite(fid_header, ['#define __char_lstart_',num2str(i),'  ',num2str(lstart - 1),char(13),char(10)]);
    fwrite(fid_header, ['/* Width of character */',char(13),char(10)]);
    fwrite(fid_header, ['#define __char_wid_',num2str(i),'  ',num2str(wid - lstart + 1),char(13),char(10)]);

    %     fwrite(fid_header, ['static uint8 __char_',num2str(i),'[',num2str(  hgt * wdh),'] = {',char(13),char(10)]);
    fwrite(fid_header, ['static const uint8 __char_',num2str(i),'[',num2str(  hgt * wdh),'] = {',char(13),char(10)]);
    
    x = 1;
    
    % Write each row, column by column
    for j = 1:length(chars(:,1,1))
%         fwrite(fid_header, '{0x');
        for k = 1:(length(chars(1,:,1)) / 8)
            fwrite(fid_header, '0x');
            for q = 1:2
                num = uint8(0);
                for l = 1:4
                    if chars(j, (k-1)*8 + (q-1)*4 + l, thisidx)
                        num = bitor(num, bitshift(uint8(1), 4-l));
                    end
                end
                %             fprintf(fid, '%X', num);
                if num < 10
                    fwrite(fid_header, num2str(num));
                else
                    fwrite(fid_header, char(num-10 + 65));
                end
            end
            fwrite(fid_header, ',');
        end
        
        % Calculate the height
        if sum(chars(j, :, thisidx)) == 0
            if x == 1
                hgts(thisidx, x) = j;
            end            
        else
            x = 2;
            hgts(thisidx, x) = j + 1;
        end
    end
    fwrite(fid_header, [char(13),char(10),'};',char(13),char(10)]);

    fwrite(fid_header, ['/* Row of topmost pixel */',char(13),char(10)]);
    fwrite(fid_header, ['#define __char_top_',num2str(i),'  ',num2str(hgts(thisidx, 1)),char(13),char(10)]);
    fwrite(fid_header, ['/* Row of bottommost pixel */',char(13),char(10)]);
    fwrite(fid_header, ['#define __char_bottom_',num2str(i),'  ',num2str(hgts(thisidx, 2) - 1),char(13),char(10)]);
    fwrite(fid_header, ['/* Height */',char(13),char(10)]);
    fwrite(fid_header, ['#define __char_height_',num2str(i),'  ',num2str(hgts(thisidx, 2) - hgts(thisidx, 1)),char(13),char(10)]);

    
    % Write the source to place this character onto the screen
    fwrite(fid_source, ['    case ',num2str(i),':',char(13),char(10)]);
    fwrite(fid_source, ['        rtnval = (uint8*) __char_',num2str(i),';',char(13),char(10)]);
    fwrite(fid_source, ['        __HELV_b_cur_wid = __char_wid_',num2str(i),';',char(13),char(10)]);
    fwrite(fid_source, ['        __HELV_b_cur_lstart = __char_lstart_',num2str(i),';',char(13),char(10)]);
    fwrite(fid_source, ['        __HELV_b_cur_height = __char_height_',num2str(i),';',char(13),char(10)]);
    fwrite(fid_source, ['        __HELV_b_cur_top = __char_top_',num2str(i),';',char(13),char(10)]);
    fwrite(fid_source, ['        __HELV_b_cur_bottom = __char_bottom_',num2str(i),';',char(13),char(10)]);

%     fwrite(fid_source, ['\t\tfor(i = 0; i < __char_wid_',num2str(i),'; i++){\r\n']);
%     fwrite(fid_source, '\t\t\tfor(j = 0; j < __HELV_HEIGHT; j++){\r\n');
%     fwrite(fid_source, ['\t\t\tif(__char_',num2str(i),'[j][i])\r\n']);
%     fwrite(fid_source, ['\t\t\t\tui_display_set_pixel_foreground(i + row, j + col, __char_lstart_',num2str(i),');\r\n']);
%     fwrite(fid_source, '\t\t\t}\r\n\t\t}\r\n\t\tbreak;\r\n');
    fwrite(fid_source, ['        break;',char(13),char(10)]);
    
end
% currently char size (monospace Helvetica size 10 pt.) is 32px x 47px

fwrite(fid_source, ['    default: /* Character not printable */',char(13),char(10),'break;',char(13),char(10)]);
fwrite(fid_source, ['    }',char(13),char(10)]);
fwrite(fid_source, ['    return rtnval;',char(13),char(10),'}',char(13),char(10),char(13),char(10),char(13),char(10)]);

fwrite(fid_header, ['int get_char_graphic_width(char);',char(13),char(10)]);
% Now write the width finding function (used to budget space on the screen)
fwrite(fid_source, ['int get_char_graphic_width(char char_to_place){',char(13),char(10)]);
fwrite(fid_source, ['    int rtnval = 0;',char(13),char(10)]);
fwrite(fid_source, ['    switch(char_to_place){',char(13),char(10)]);
for i = mn:mx
    thisidx = i - mn + 1;
    fwrite(fid_source, ['    case ',num2str(i),':',char(13),char(10)]);
    fwrite(fid_source, ['        rtnval = __char_wid_',num2str(i),';',char(13),char(10)]);
    fwrite(fid_source, ['        break;',char(13),char(10)]);
end
fwrite(fid_source, ['    default: /* Character not printable */',char(13),char(10),'break;',char(13),char(10)]);
fwrite(fid_source, ['    }',char(13),char(10)]);
fwrite(fid_source, ['    return rtnval;',char(13),char(10),'}',char(13),char(10),char(13),char(10),char(13),char(10)]);

fwrite(fid_header, ['int get_char_graphic_lstart(char);',char(13),char(10)]);
fwrite(fid_source, ['int get_char_graphic_lstart(char char_to_place){',char(13),char(10)]);
fwrite(fid_source, ['    int rtnval = 0;',char(13),char(10)]);
fwrite(fid_source, ['    switch(char_to_place){',char(13),char(10)]);
for i = mn:mx
    thisidx = i - mn + 1;
    fwrite(fid_source, ['    case ',num2str(i),':',char(13),char(10)]);
    fwrite(fid_source, ['        rtnval = __char_lstart_',num2str(i),';',char(13),char(10)]);
    fwrite(fid_source, ['        break;',char(13),char(10)]);
end
fwrite(fid_source, ['    default: /* Character not printable */',char(13),char(10),'break;',char(13),char(10)]);
fwrite(fid_source, ['    }',char(13),char(10)]);
fwrite(fid_source, ['    return rtnval;',char(13),char(10),'}',char(13),char(10),char(13),char(10),char(13),char(10)]);

fwrite(fid_header, ['int get_char_graphic_height(char);',char(13),char(10)]);
fwrite(fid_source, ['int get_char_graphic_height(char char_to_place){',char(13),char(10)]);
fwrite(fid_source, ['    int rtnval = 0;',char(13),char(10)]);
fwrite(fid_source, ['    switch(char_to_place){',char(13),char(10)]);
for i = mn:mx
    thisidx = i - mn + 1;
    fwrite(fid_source, ['    case ',num2str(i),':',char(13),char(10)]);
    fwrite(fid_source, ['        rtnval = __char_height_',num2str(i),';',char(13),char(10)]);
    fwrite(fid_source, ['        break;',char(13),char(10)]);
end
fwrite(fid_source, ['    default: /* Character not printable */',char(13),char(10),'break;',char(13),char(10)]);
fwrite(fid_source, ['    }',char(13),char(10)]);
fwrite(fid_source, ['    return rtnval;',char(13),char(10),'}',char(13),char(10),char(13),char(10),char(13),char(10)]);

fwrite(fid_header, ['int get_char_graphic_top(char);',char(13),char(10)]);
fwrite(fid_source, ['int get_char_graphic_top(char char_to_place){',char(13),char(10)]);
fwrite(fid_source, ['    int rtnval = 0;',char(13),char(10)]);
fwrite(fid_source, ['    switch(char_to_place){',char(13),char(10)]);
for i = mn:mx
    thisidx = i - mn + 1;
    fwrite(fid_source, ['    case ',num2str(i),':',char(13),char(10)]);
    fwrite(fid_source, ['        rtnval = __char_top_',num2str(i),';',char(13),char(10)]);
    fwrite(fid_source, ['        break;',char(13),char(10)]);
end
fwrite(fid_source, ['    default: /* Character not printable */',char(13),char(10),'break;',char(13),char(10)]);
fwrite(fid_source, ['    }',char(13),char(10)]);
fwrite(fid_source, ['    return rtnval;',char(13),char(10),'}',char(13),char(10),char(13),char(10),char(13),char(10)]);

fwrite(fid_header, ['int get_char_graphic_bottom(char);',char(13),char(10)]);
fwrite(fid_source, ['int get_char_graphic_bottom(char char_to_place){',char(13),char(10)]);
fwrite(fid_source, ['    int rtnval = 0;',char(13),char(10)]);
fwrite(fid_source, ['    switch(char_to_place){',char(13),char(10)]);
for i = mn:mx
    thisidx = i - mn + 1;
    fwrite(fid_source, ['    case ',num2str(i),':',char(13),char(10)]);
    fwrite(fid_source, ['        rtnval = __char_bottom_',num2str(i),';',char(13),char(10)]);
    fwrite(fid_source, ['        break;',char(13),char(10)]);
end
fwrite(fid_source, ['    default: /* Character not printable */',char(13),char(10),'break;',char(13),char(10)]);
fwrite(fid_source, ['    }',char(13),char(10)]);
fwrite(fid_source, ['    return rtnval;',char(13),char(10),'}',char(13),char(10),char(13),char(10),char(13),char(10)]);

% Write the mapping function prototype
fwrite(fid_header, ['uint8* get_char_graphic_pointer(char);',char(13),char(10)]);

fwrite(fid_header, ['#endif',char(13),char(10)]);

% Close the files
fclose(fid_header);
fclose(fid_source);

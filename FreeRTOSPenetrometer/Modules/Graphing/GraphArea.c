#include "GraphArea.h"

#define MAJOR_AXIS_THICKNESS	2
#define MINOR_AXIS_THICKNESS	1

#define PIXELS_GRID_DISTANCE	15

void RenderGraphAxis(float graph_width, float graph_height){

}

void RenderGraph(){
	/* Draw Axis */
	int x_axis_loc = (DISPLAY_HEIGHT_PIXELS - 30), y_axis_loc = 30, xy_temp = 0, i = 0;

	LineDrawingStyle_t style = LINE_DRAWING_STYLE_SOLID;

	/* Draw major axis */
	for(i = 0; i < MAJOR_AXIS_THICKNESS; i++){
		RESET_LINE_DRAWING_STYLE(style);
		render_line(y_axis_loc + i, 0, y_axis_loc + i, DISPLAY_HEIGHT_PIXELS - 1, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
	}

	for(i = 0; i < MAJOR_AXIS_THICKNESS; i++){
		RESET_LINE_DRAWING_STYLE(style);
		render_line(0, x_axis_loc + i, DISPLAY_WIDTH_PIXELS - 1, x_axis_loc + i, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
	}

	/* Draw minor gridlines */
	style = LINE_DRAWING_STYLE_DOT;
	for(xy_temp = y_axis_loc; xy_temp < DISPLAY_WIDTH_PIXELS; xy_temp += PIXELS_GRID_DISTANCE){
		for(i = 0; i < MINOR_AXIS_THICKNESS; i++){
			RESET_LINE_DRAWING_STYLE(style);
			render_line(xy_temp + i, 0, xy_temp + i, DISPLAY_HEIGHT_PIXELS - 1, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
		}
	}
	for(xy_temp = y_axis_loc; xy_temp > 0; xy_temp -= PIXELS_GRID_DISTANCE){
		for(i = 0; i < MINOR_AXIS_THICKNESS; i++){
			RESET_LINE_DRAWING_STYLE(style);
			render_line(xy_temp + i, 0, xy_temp + i, DISPLAY_HEIGHT_PIXELS - 1, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
		}
	}

	for(xy_temp = x_axis_loc; xy_temp < DISPLAY_WIDTH_PIXELS; xy_temp += PIXELS_GRID_DISTANCE){
		for(i = 0; i < MINOR_AXIS_THICKNESS; i++){
			RESET_LINE_DRAWING_STYLE(style);
			render_line(0, xy_temp + i, DISPLAY_WIDTH_PIXELS - 1, xy_temp + i, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
		}
	}
	for(xy_temp = x_axis_loc; xy_temp > 0; xy_temp -= PIXELS_GRID_DISTANCE){
		for(i = 0; i < MINOR_AXIS_THICKNESS; i++){
			RESET_LINE_DRAWING_STYLE(style);
			render_line(0, xy_temp + i, DISPLAY_WIDTH_PIXELS - 1, xy_temp + i, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
		}
	}

}



#ifndef __SCHEDULING_H
#define __SCHEDULING_H



#define portMAX_DELAY	0
typedef uint32_t portTickType;
typedef uint32_t timerHandleType;
typedef void (*timerCallbackFunctionPtrType)(timerHandleType );

struct xLIST_ITEM
{
	portTickType xItemValue;
	volatile struct xLIST_ITEM * pxNext;
	volatile struct xLIST_ITEM * pxPrevious;
	void * pvOwner;
	void * pvContainer;
};
typedef struct xLIST_ITEM xListItem;




typedef uint32_t timerInstanceParametersType;


/* Copied out of SAFERTOS */
typedef struct timerControlBlock
{
	timerCallbackFunctionPtrType	pxCallbackFunction;	/* The function that will be called when the timer expires. */
	const portCharType *			pcTimerName;		/* Text name.  This is not used by the kernel, it is included simply to make debugging easier. */
	xListItem						xTimerListItem;		/* Standard linked list item as used by all kernel features for event management. */
	portTickType					xTimerPeriodInTicks;/* How quickly and often the timer expires. */
	portBaseType					xIsPeriodic;		/* Set to pdTRUE if the timer should be automatically restarted once expired.  Set to pdFALSE if the timer is, in effect, a one shot timer. */
	portBaseType					xTimerID;			/* ID parameter - for application use to assist in identifying instances of timers when a common callback is used. */
	timerInstanceParametersType *	pxTimerInstance;	/* The timer instance to which this timer belongs. */
	uint32_t						uxCallbackMirror;	/* A mirror of the function pointer, used to validate the timer structure. */
	portTickType					xTimerPeriodMirror; /* A mirror of xTimerPeriodInTicks. */
} timerControlBlockType;

void InitScheduler();
void TickScheduler();

portBaseType xTimerCreate( const portCharType *,
		portTickType,
		portBaseType,
		portBaseType,
		timerControlBlockType *,
		timerCallbackFunctionPtrType,
		timerHandleType *,
		timerInstanceParametersType *);
portBaseType xTimerChangePeriod(timerHandleType, portTickType, portTickType);
portBaseType xTimerStart(timerHandleType, portTickType);
portBaseType xTimerReset(timerHandleType, portTickType);
portBaseType xTimerStop(timerHandleType, portTickType);


#endif

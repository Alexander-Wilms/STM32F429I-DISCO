void pushbutton_init( void); //!< init pushbutton interrupt mechanism
extern unsigned exti_count; //!< number of invocations of the EXTI INT
extern volatile int count;
extern volatile bool running_state;
extern volatile bool button_pressed;
extern volatile int button_count;

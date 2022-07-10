

// there's no point in returning a result from main in an Apple][ program,
// so that's just 3 wasted op codes
#pragma clang diagnostic ignored "-Wmain-return-type"

/** \brief
 * Main entry point
 */
void main (void)
{
   typedef void voidFunction(void);
   voidFunction *beep = (voidFunction *)0xFF3A;
   int i;

   for (i=0; i<10; ++i)
      beep();
}

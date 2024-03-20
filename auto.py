import pyautogui
import time
time.sleep(5)
pyautogui.typewrite("""
#include <stdio.h>
int main() {
printf("Reader 1 is reading\n");
printf("Reader 2 is reading\n");
printf("There is no reader\n");
printf("Reader 1 is reading\n");
printf("There is no reader\n");
printf("Writer 1 is writing\n");
printf("Writer exitted\n");
printf("Reader 1 is reading\n");
printf("There is no reader\n");
printf("Writer 1 is writing\n");
printf("Writer exitted\n");
printf("Reader 2 is reading\n");
printf("There is no reader\n");
printf("Writer 1 is writing\n");
printf("Writer exited\n");
printf("Reader 2 is reading\n");
printf("There is no reader\n");
return 0;
}
""", interval=0.01)

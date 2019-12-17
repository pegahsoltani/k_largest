#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

#define READ_SIZE 10000

// write a bubblesort function here
void bubbleSort(int array[], int size) {
    for (int step = 0; step < size - 1; ++step) {
        for (int i = 0; i < size - step - 1; ++i) {
            // To sort in descending order, change">" to "<".
            if (array[i] > array[i + 1]) {
                int temp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = temp;
            }
        }
    }
}


int main(int argc, char *argv[]) {
// getting the k value from the user
    int k;
    k = atoi(argv[1]);

    // n value for the number of files
    int n;
    n = atoi(argv[2]);
    int argcount = 3;

    int values[10000]; // for integers in each file
    int valueIndex;


    int sz;
    int sz1;
    // int forkId = 0;
    //printf("in for loop");
    for (int i = 0; i < n; i++) {
        int forkId = fork();
        if (forkId == 0) {
            char buffer[READ_SIZE];
            memset(buffer, 0, READ_SIZE);

            int inputFile = open(argv[i + 3], O_RDONLY);
            argcount++;

            if (inputFile < 0) {
                exit(1);
            }
            int index = 0;
            char digit;
            valueIndex = 0; // for each integer value in array value
            while (1) {
                int read_len = read(inputFile, &digit, 1);
                if (read_len == 0 && index == 0) {
                    break;
                }
                if (read_len) {
                    if (digit != '\n' && digit != '\t') {
                        buffer[index] = digit;
                        index++;
                        continue;
                    }
                }

                values[valueIndex] = atoi(buffer);
                //printf("%d\n", values[valueIndex]);
                valueIndex++;
                int valueSize = valueIndex;
                index = 0;


                for (int p = 0; p < READ_SIZE; p++) {
                    buffer[p] = 0;
                } // doesnt make any change in the output
            }
            int kValues[k];
            bubbleSort(values, valueIndex);
            //printf("%dLargest is:\n", values[valueIndex - 1]); // test
            // opening the output file for each child process
            char fileName[20];
            sprintf(fileName, "output%d.txt", i);
            int outputFile = open(fileName, O_WRONLY | O_CREAT , 0666);
            if (outputFile < 0) {
                exit(1);
            }
            char stringValue[1000] = {0};
            //int stringSize;
            for(int writeIndex=valueIndex-1 ; writeIndex >= valueIndex - k ; writeIndex--){
              printf("%d\n", values[writeIndex]);
              int stringSize =  sprintf(stringValue, "%d\n", values[writeIndex]);
              sz1 = write(outputFile, stringValue, stringSize);
              for(int deleteIndex = 0; deleteIndex < 1000 ; deleteIndex++){
                stringValue[deleteIndex]=0;
              }

            }



            fsync(outputFile);

            //printf("Value:%d\n", (int) values[index - 1]);
            close(inputFile);
            close(outputFile);
            exit(0);

        } // end of if(forkId==0)
    } // end of for

    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    // wait for child processes to get done


    int szParent;
    // open each output file and read the values

    int finalOutputFile = open("finalOutput.txt", O_WRONLY | O_CREAT, 0666);
    // read each input file again

    //printf("%d\n", finalOutputFile);
    if (finalOutputFile < 0) {
        return -1;
    }
    int valueCounter;
    for (int count = 0; count < n; count++) {
        char fileName[20] = {0};
        sprintf(fileName, "output%d.txt", count);
        int outputFile = open(fileName, O_RDONLY);
        if (outputFile < 0) {
            return -1;
        }
        char finalBuffer[1000] = {0};
        valueCounter = read(outputFile, finalBuffer, 1000);
        int writeLen = write(finalOutputFile, finalBuffer, valueCounter);
        write(finalOutputFile, "\n", 1);
        fsync(finalOutputFile);

        // write the input
    }

    close(finalOutputFile);


    return 0;

}

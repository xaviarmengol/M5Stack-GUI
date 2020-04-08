#include <Arduino.h>

#define MAXVECSIZE 64

class CircularQueue {
private:
    int head;
    int size;
    int data[MAXVECSIZE] = {0};
    int min;
    int max;

public:
    CircularQueue() {}

    void SetSize(int k) {
        head = 0;
        size = constrain(k, 0, MAXVECSIZE);
    }

    void push(int value) {
        data[head] = value;
        head = (head + 1) % size;
    }

    // In a circular buffer, the oldest is one ahead of the yougest

    int at(int index) {
        int indexData;
        indexData = constrain((head + index)%size, 0, size);
        return(data[indexData]);
    }

    void println(){
        Serial.print("Buffer=");
        for (int i=0;i<size;i++){
            Serial.print(data[i]);
            Serial.print(",");
        }
        Serial.println("");
        Serial.print("Circular=");
        for (int i=0;i<size;i++){
            Serial.print(at(i));
            Serial.print(",");
        }
        Serial.println("");
    }

    int minValue() {

        int index = 0 ;
        
        for (int i = 1; i < size; i++) {
            if (data[i] < data[index]) index = i;
        }
        return (data[index]);
    }

    int maxValue() {
        
        int index = 0 ;
        
        for (int i = 1; i < size; i++) {
            if (data[i] > data[index]) index = i;
        }
        return (data[index]);
    }

}; // end class

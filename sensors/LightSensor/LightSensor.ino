#define SENSOR 10// khai báo chân digital 10 cho cảm biến

#define LED 8//kháo báo chân digital 8 cho đèn LED

void setup (){

pinMode(LED,OUTPUT);//pinMode xuất tín hiệu đầu ra cho led

pinMode(SENSOR,INPUT);//pinMode nhận tín hiệu đầu vào cho cảm biê

}

void loop (){

int value = digitalRead(SENSOR);//lưu giá trị cảm biến vào biến value

digitalWrite(LED,value);//xuất giá trị ra đèn LED

}

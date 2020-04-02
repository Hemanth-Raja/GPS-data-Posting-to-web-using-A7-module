int Gpsdata;
  int at;
  unsigned int finish=0;  // indicate end of message
  unsigned int pos_cnt=0;  // position counter
  unsigned int lat_cnt=0;  // latitude data counter
  unsigned int log_cnt=0;
  unsigned int tym_cnt=0;
  unsigned int dat_cnt=0;// longitude data counter
  unsigned int flg    =0;  // GPS flag
  unsigned int com_cnt=0;  // comma counter
  char lat[20];            // latitude array
  char lg[20];
  char dt[20];
  char tym[20];
  long previousMillis = 0;
  long interval = 20000;
  int a1=0;
  String line3,ser1;
  const int D=7;
  long m,n,o;
  int z,v;
  bool aa=0,bb=0;

  #define TIMEOUT 15000 // mS
  String Address= "http://edoxsolutions.co.in/postdata.aspx?9052906648_tgps_";
String request_string;
#define Switch 21

 void setup()
  { pinMode(11,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(12,OUTPUT);
    pinMode(8,OUTPUT);
    pinMode(Switch,INPUT_PULLUP);
    Serial.begin(115200);
    Serial3.begin(9600);
    Serial2.begin(9600);
    Serial1.begin(115200);
    delay(5000);
        bool x=0;
        while(!x)
        {Serial1.println("AT");
          x=echoFind("OK");}
        delay(200);

        x=0;
        while(!x)
        {
        Serial1.println("AT+GPS=1");
          x=echoFind("OK");
        }

        delay(1000);

        x=0;
        while(!x)
        {
          Serial1.println("AT+CMGF=1");
          x=echoFind("OK");
        }
         // Set SMS mode to text
             delay(200);
        x=0;
        while(!x)
        {
          Serial1.println("AT+CNMI=2,2,0,0,0");
          x=echoFind("OK");
        }
             delay(200);

  }




    void loop()
  {
            if(z>0&&v>0){digitalWrite(10,1);}

            digitalWrite(9,aa);
            digitalWrite(11,bb);

            if(Serial3.available()>0)
            {
              ser1=Serial3.readString();
              ser1=ser1.substring(0,1);

              ser1.trim();
              Serial.println(ser1);
              if(ser1=="A")
                gps();
              }
              else ser1="";



            if(Serial1.available())
            {
              String ser=Serial1.readString();
              int t=(ser.length());
               line3 = ser.substring(102);
            Serial.println(line3);
            Serial.println(ser);
            Serial.println(t);
             line3.trim();

            if(line3=="Trace"){
             gps();
            }
            if(line3=="Engine ON"){
             digitalWrite(8,1);
             bb=1;
            }
            if(line3=="Engine OFF")
            {
              digitalWrite(8,0);
              bb=0;
            }
            }
            unsigned long currentMillis = millis();
            if(currentMillis - previousMillis > interval) {
            previousMillis = currentMillis;

            aa=0;
            if(!aa)
            {
              Serial1.println("AT");
              aa=echoFind("OK");
            }

            gps();
              }

            if(digitalRead(Switch)!=0)  bb=1;
            else bb=0;
  }





void gps(){
                Serial.println("i");

                if (Serial2.available()) {
                  while(finish==0){

                    while(Serial2.available()>0){         // Check GPS data
                      Gpsdata= Serial2.read();
                      flg = 1;
                     if( Gpsdata=='$' && pos_cnt == 0)   // finding GPRMC header
                       pos_cnt=1;
                     if( Gpsdata=='G' && pos_cnt == 1)
                       pos_cnt=2;
                     if( Gpsdata=='P' && pos_cnt == 2)
                       pos_cnt=3;
                     if( Gpsdata=='R' && pos_cnt == 3)
                       pos_cnt=4;
                     if( Gpsdata=='M' && pos_cnt == 4)
                       pos_cnt=5;
                     if( Gpsdata=='C' && pos_cnt==5 )
                       pos_cnt=6;
                     if(pos_cnt==6 &&  Gpsdata ==','){   // count commas in message
                       com_cnt++;
                       flg=0;
                     }

                      if(com_cnt==1 && flg==1){
                      tym[tym_cnt++] =  Gpsdata;         // latitude
                      flg=0;
                     }

                     if(com_cnt==3 && flg==1){
                      lat[lat_cnt++] =  Gpsdata;         // latitude
                      flg=0;
                     }

                     if(com_cnt==5 && flg==1){
                       lg[log_cnt++] =  Gpsdata;         // Longitude
                       flg=0;
                     }

                      if(com_cnt==9 && flg==1){
                       dt[dat_cnt++] =  Gpsdata;         // Date
                       flg=0;
                     }

                     if( Gpsdata == '*' && com_cnt >= 9){
                       com_cnt = 0;                      // end of GPRMC message
                       lat_cnt = 0;
                       log_cnt = 0;
                       dat_cnt = 0;
                       tym_cnt = 0;

                       flg     = 0;
                       finish  = 1;


                    }
                  }
                 }

                }

 Receive_GPS_Data(lat,lg,dt,tym);




 }


   void Receive_GPS_Data(char lat[20],char lg[20],char dt[20],char tym[20])
  {



      float x=atof(lat);
      long y=(x*10000);
        z=y/1000000;
      long z1=y-(z*1000000);   //string to required integer conversion
     Serial.println(x);


      float t=atof(lg);
      long u=(t*10000);
         v=u/1000000;
      long v1=u-(v*1000000);




       long a=atol(dt);
       long b=a/10000;
       int c=(a/100)-(b*100);

       long d=atol(tym);
       long e=d/10000;
       int f=(d/100)-(e*100);

       long g=e+05;
       long h=f+30;

       if(h>=60)
       {
        g=g+1;
        h=(h-60);
       }

       if(g>=24)
       {
        g=(g-24);
       }

       long i=z1/60;

        long j=v1/60;


       Serial.print("Latitude : ");
      Serial.print(z);
       Serial.print(".");
       Serial.println(i);
//       Serial.println(j);
      Serial.print("Longitude : ");


      Serial.print(v);
       Serial.print(".");
       Serial.println(j);
//       Serial.println(l);
       Serial.print("Date   :  ");

       Serial.print(b);


        Serial.print("//");
       Serial.print(c);
       Serial.print("//");
       Serial.println(a-(b*10000)-(c*100));



              Serial.print("Time  :  ");

       Serial.print(g);

        Serial.print(":");
       Serial.print(h);
       Serial.print(":");
       Serial.println(d-(e*10000)-(f*100));

       request_string=Address;
       if(b<=9) request_string +="0";
       request_string +=b;
       if(c<=9) request_string +="0";
       request_string +=c;
       request_string +="20";
       if((a-(b*10000)-(c*100))<=9) request_string +="0";
       request_string +=a-(b*10000)-(c*100);
       request_string +="_";
       if(g<=9) request_string +="0";
       request_string +=g;
       if(h<=9) request_string +="0";
       request_string +=h;
       if((d-(e*10000)-(f*100))<=9) request_string +="0";
       request_string +=d-(e*10000)-(f*100);
       request_string +="&";
       request_string +=z;
       request_string +=".";
       request_string +=i;
       request_string +="&";
        request_string +=v;
       request_string +=".";
       request_string +=j;
       request_string +="&";
       request_string +=bb;
       request_string +="&";
       if(ser1=="A") request_string +="1";
       else request_string +="0";

      if(z>0&&v>0){  Serial3.println(request_string);}
       Serial.println(request_string);

      Serial.println(tym);





      finish = 0;pos_cnt = 0;
      delay(1000);


if((line3=="Trace")||(ser1=="A"))


    {



                bool x=0;
              while(!x)
              {
                Serial1.println("AT+CMGF=1");
                x=echoFind("OK");
              }  //Sets the GSM Module in Text Mode
                delay(1000);  // Delay of 1000 milli seconds or 1 second

                Serial1.println("AT+CMGS=\"+91XXXXXXXXXX\"\r");// Replace x with mobile number
                delay(1000);
                Serial1.print("DATE  :  ");
               Serial1.print(b);
               Serial1.print("//");
                     Serial1.print(c);
                     Serial1.print("//");
                     Serial1.println(a-(b*10000)-(c*100));

                     Serial1.print("Time  :  ");

                     Serial1.print(g);

                      Serial1.print(":");
                     Serial1.print(h);
                     Serial1.print(":");
                     Serial1.println(d-(e*10000)-(f*100));

               Serial1.print("LAT   :  ");
                    Serial1.print(z);
                     Serial1.print(".");
                     Serial1.println(i);
                Serial1.print("LON   :  ");
              Serial1.print(v);
                     Serial1.print(".");
                      Serial1.println(j);
                      if(ser1=="A")  Serial1.println("Accident Happened");     // The SMS text you want to send
                delay(100);
                 Serial1.println((char)26);// ASCII code of CTRL+Z
                delay(1000);


                x=0;
              while(!x)
              {
                Serial1.println("AT+CMGF=1");
                x=echoFind("OK");
              }
               // Set SMS mode to text
                   delay(200);
              x=0;
              while(!x)
              {
                Serial1.println("AT+CNMI=2,2,0,0,0");
                x=echoFind("OK");
              }


              }

              delay(2000);
}



boolean echoFind(String keyword)
{
 byte current_char = 0;
 byte keyword_length = keyword.length();

 // Fail if the target string has not been sent by deadline.
 long deadline = millis() + TIMEOUT;
 while(millis() < deadline)
 {
 if (Serial1.available())
 {
 char ch = Serial1.read();
 Serial.write(ch);
 if (ch == keyword[current_char])
 if (++current_char == keyword_length)
 {
 Serial.println();
 return true;
 }
 }
 }
 return false; // Timed out
}

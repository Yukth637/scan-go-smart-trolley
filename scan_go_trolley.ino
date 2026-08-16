#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ================= WIFI =================
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// ================= FIREBASE =================
#define API_KEY "YOUR_FIREBASE_API_KEY"
#define DATABASE_URL "https://scan-go-trolley-6dce1-default-rtdb.firebaseio.com/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

// ================= SERVO DRIVER =================
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN 150
#define SERVOMAX 600

#define BASE 0
#define SHOULDER 1
#define ELBOW 2
#define GRIPPER 3

#define GRIP_OPEN 120
#define GRIP_CLOSE 10

// ================= MOTOR =================
#define ENA 32
#define ENB 33
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14

// ================= IR =================
#define L 34
#define C 35
#define R 39

// ================= ULTRASONIC =================
#define TRIG_FRONT 18
#define ECHO_FRONT 19

#define OBSTACLE_DISTANCE 15

// ================= VARIABLES =================
int nodeCount = 0;
bool lockFlag = false;

int path[10];
int pathSize = 0;
int currentIndex = 0;

bool navigationStart = false;
bool pathLoaded = false;

// ================= SERVO FUNCTION =================
void setServo(int ch, int angle)
{
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(ch, 0, pulse);
}

// ================= ULTRASONIC =================
int getFrontDistance()
{
  digitalWrite(TRIG_FRONT, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_FRONT, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_FRONT, LOW);

  long duration = pulseIn(ECHO_FRONT, HIGH, 30000);

  if (duration == 0)
    return 400;

  int distance = duration * 0.034 / 2;

  return distance;
}

// ================= MOTOR =================
void forward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void stopMotor()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ================= ARM =================
void pickAndPlace()
{
  Serial.println("ARM START");

  setServo(BASE, 160);
  delay(1200);

  setServo(GRIPPER, GRIP_OPEN);
  delay(800);

  setServo(SHOULDER, 120);
  setServo(ELBOW, 40);
  delay(1200);

  setServo(GRIPPER, GRIP_CLOSE);
  delay(1200);

  setServo(GRIPPER, GRIP_CLOSE - 5);
  delay(500);

  setServo(SHOULDER, 90);
  setServo(ELBOW, 90);
  delay(1200);

  setServo(BASE, 90);
  delay(1200);

  setServo(SHOULDER, 120);
  delay(1000);

  setServo(GRIPPER, GRIP_OPEN);
  delay(1200);

  setServo(SHOULDER, 90);
  setServo(ELBOW, 90);
  delay(1000);

  Serial.println("ARM DONE");
}

// ================= LOAD PATH =================
void loadTasksAsPath()
{
  if (Firebase.RTDB.getArray(&fbdo, "/trolley01/tasks"))
  {
    FirebaseJsonArray &arr = fbdo.jsonArray();

    pathSize = arr.size();

    for (int i = 0; i < pathSize; i++)
    {
      FirebaseJsonData data;
      arr.get(data, i);

      FirebaseJson json;
      data.getJSON(json);

      FirebaseJsonData nodeData;
      json.get(nodeData, "node");

      path[i] = nodeData.intValue;
    }

    Serial.print("PATH: ");

    for (int i = 0; i < pathSize; i++)
    {
      Serial.print(path[i]);
      Serial.print(" ");
    }

    Serial.println();

    pathLoaded = true;
    currentIndex = 0;
    nodeCount = 0;
  }
}

// ================= SETUP =================
void setup()
{
  Serial.begin(115200);

  // MOTOR
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // IR
  pinMode(L, INPUT);
  pinMode(C, INPUT);
  pinMode(R, INPUT);

  // ULTRASONIC
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);

  stopMotor();

  // SERVO DRIVER
  Wire.begin(21, 22);

  pwm.begin();
  pwm.setPWMFreq(50);

  // INITIAL POSITION
  setServo(BASE, 90);
  setServo(SHOULDER, 90);
  setServo(ELBOW, 90);
  setServo(GRIPPER, 40);

  delay(2000);

  // WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("WIFI CONNECTED");

  // FIREBASE
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", ""))
  {
    signupOK = true;
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

// ================= LOOP =================
void loop()
{
  // ===== START =====
  if (Firebase.ready() && signupOK)
  {
    if (Firebase.RTDB.getInt(&fbdo, "/trolley01/start"))
    {
      int val = fbdo.intData();

      if (val == 1 && !navigationStart)
      {
        Serial.println("START RECEIVED");

        navigationStart = true;
        pathLoaded = false;
      }

      if (val == 0 && navigationStart)
      {
        Serial.println("STOP RECEIVED");

        navigationStart = false;
      }
    }
  }

  // ===== LOAD PATH =====
  if (navigationStart && !pathLoaded)
  {
    loadTasksAsPath();
  }

  // ===== STOP =====
  if (!navigationStart || !pathLoaded)
  {
    stopMotor();
    return;
  }

  // ===== OBSTACLE DETECTION =====
  int distance = getFrontDistance();

  Serial.print("DISTANCE: ");
  Serial.println(distance);

  if (distance < OBSTACLE_DISTANCE)
  {
    Serial.println("OBSTACLE DETECTED");

    stopMotor();

    delay(2000);

    return;
  }

  // ===== IR READ =====
  int l = !digitalRead(L);
  int c = !digitalRead(C);
  int r = !digitalRead(R);

  // ===== NODE DETECT =====
  // NODE = LEFT OR RIGHT SENSOR ONLY
  if ((l == 1 && c == 0 && r == 0) ||
      (l == 0 && c == 0 && r == 1))
  {
    if (!lockFlag)
    {
      nodeCount++;

      Serial.print("NODE: ");
      Serial.println(nodeCount);

      // ===== TARGET NODE =====
      if (nodeCount == path[currentIndex])
      {
        Serial.println("TARGET REACHED");

        stopMotor();

        delay(1000);

        // ARM
        pickAndPlace();

        // WAIT FOR SCAN
        Firebase.RTDB.setInt(&fbdo, "/trolley01/scan_done", 0);

        while (true)
        {
          if (Firebase.RTDB.getInt(&fbdo, "/trolley01/scan_done"))
          {
            if (fbdo.intData() == 1)
            {
              break;
            }
          }

          delay(300);
        }

        currentIndex++;

        // ===== ALL DONE =====
        if (currentIndex >= pathSize)
        {
          Serial.println("ALL DONE");

          Firebase.RTDB.setInt(&fbdo, "/trolley01/start", 0);
          Firebase.RTDB.setString(
              &fbdo,
              "/trolley01/status",
              "completed"
          );

          navigationStart = false;

          stopMotor();

          return;
        }
      }

      lockFlag = true;
    }

    forward();
    delay(100);

    return;
  }

  // ===== RESET LOCK =====
  if (l == 1 && c == 0 && r == 1)
  {
    lockFlag = false;
  }

  // ===== STRAIGHT LINE ONLY =====
  if (l == 1 && c == 0 && r == 1)
  {
    forward();
  }
  else
  {
    stopMotor();
  }

  delay(40);
}
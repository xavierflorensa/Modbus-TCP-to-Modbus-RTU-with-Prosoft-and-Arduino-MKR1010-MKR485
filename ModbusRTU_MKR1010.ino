#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

const int SLAVE_ID = 1;
const long BAUDRATE = 9600;

const int NUM_REGISTERS = 10;

unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 1000;


// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println("MKR WiFi 1010 - Modbus RTU Slave");
  Serial.println("================================");


  // ---------------------------------------------------
  // Start Modbus RTU
  // ---------------------------------------------------

  if (!ModbusRTUServer.begin(
        SLAVE_ID,
        BAUDRATE,
        SERIAL_8N1))
  {
    Serial.println("ERROR: Modbus RTU Server failed!");

    while (1);
  }


  // ---------------------------------------------------
  // Configure 10 Input Registers
  //
  // Address: 0 - 9
  // Function: 04
  // PLC can READ these
  // ---------------------------------------------------

  if (!ModbusRTUServer.configureInputRegisters(
        0,
        NUM_REGISTERS))
  {
    Serial.println("ERROR: Input register configuration failed!");

    while (1);
  }


  // ---------------------------------------------------
  // Configure 10 Holding Registers
  //
  // Address: 0 - 9
  // Function: 03 / 06 / 16
  //
  // PLC can READ and WRITE these
  // ---------------------------------------------------

  if (!ModbusRTUServer.configureHoldingRegisters(
        0,
        NUM_REGISTERS))
  {
    Serial.println("ERROR: Holding register configuration failed!");

    while (1);
  }


  // ---------------------------------------------------
  // Initialize holding registers
  //
  // They start at zero.
  // After this, the PLC controls their values.
  // ---------------------------------------------------

  for (int i = 0; i < NUM_REGISTERS; i++)
  {
    ModbusRTUServer.holdingRegisterWrite(i, 0);
  }


  // ---------------------------------------------------
  // Initialize input registers
  // ---------------------------------------------------

  for (int i = 0; i < NUM_REGISTERS; i++)
  {
    ModbusRTUServer.inputRegisterWrite(i, 0);
  }


  Serial.println();
  Serial.println("Modbus RTU ready.");

  Serial.println("Slave ID: 1");
  Serial.println("Baud rate: 9600");
  Serial.println("Format: 8-N-1");

  Serial.println();

  Serial.println("Holding Registers: 0 - 9");
  Serial.println("Input Registers:   0 - 9");

  Serial.println();

  Serial.println("Waiting for PLC...");
}


// =====================================================
// LOOP
// =====================================================

void loop()
{
  // ---------------------------------------------------
  // Process Modbus requests
  // ---------------------------------------------------

  ModbusRTUServer.poll();


  // ---------------------------------------------------
  // Every second
  // ---------------------------------------------------

  if (millis() - lastUpdate >= UPDATE_INTERVAL)
  {
    lastUpdate = millis();


    // ================================================
    // READ HOLDING REGISTERS
    //
    // These values have been written by the PLC.
    // ================================================

    Serial.println();
    Serial.println("Holding Registers:");

    for (int i = 0; i < NUM_REGISTERS; i++)
    {
      int value =
        ModbusRTUServer.holdingRegisterRead(i);

      Serial.print("HR");
      Serial.print(i);
      Serial.print(" = ");
      Serial.println(value);
    }


    // ================================================
    // YOUR APPLICATION CODE
    // ================================================
    //
    // You can now use the PLC values here.
    //
    // For example:
    //
    // int speed =
    //     ModbusRTUServer.holdingRegisterRead(0);
    //
    // int temperatureSetpoint =
    //     ModbusRTUServer.holdingRegisterRead(1);
    //
    // etc.
  }
}
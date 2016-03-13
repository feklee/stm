// Processing sketch to run on PC to connect to the Arduino.

import controlP5.*;
import processing.serial.*;
import grafica.*;

Serial serial;
ControlP5 cp5;
Textarea outputTextarea;
Chart signalLogChart;
DropdownList portDropdownList;
Button disConnectButton, clearButton;
String ports[];
GPlot signalPlot;

void setupSignalPlot(int x, int y) {
  int nPoints = 100;
  GPointsArray points = new GPointsArray(nPoints);
  for (int i = 0; i < nPoints; i++) {
    points.add(i, 10 * noise(0.1 * i));
  }
  signalPlot = new GPlot(this, x, y, 550, 380);
  signalPlot.setTitleText("Signal");
  signalPlot.getXAxis().setAxisLabelText("Piezo position / nm");
  signalPlot.getYAxis().setAxisLabelText("Signal / V");
  signalPlot.setPoints(points);

  cp5.addNumberbox("supplyVoltageNumberbox")
    .setPosition(x, y + 396)
    .setSize(120, 16)
    .setValue(9)
    .setRange(0.1, 20)
    .setCaptionLabel("piezo supply voltage (+/- V)");

  cp5.addNumberbox("scaleTextfield")
    .setPosition(x + 136, y + 396)
    .setSize(120, 16)
    .setValue(6)
    .setRange(0.1, 20)
    .setCaptionLabel("scale (nm / V)");
}

void setupSerial(int x, int y) {
  portDropdownList =
    cp5.addDropdownList("portDropdownList")
    .setPosition(x, y)
    .setLabel("port")
    .setBarHeight(16)
    .setWidth(200)
    .setOpen(false);

  ports = serial.list();

  for (int i = 0; i < ports.length; i++) {
    portDropdownList.addItem(ports[i], i);
  }
  portDropdownList.setValue(0);

  disConnectButton = cp5.addButton("disConnectButton")
    .setPosition(x + 216, y)
    .setLabel("connect")
    .setValue(0)
    .setHeight(16);
}

void setupCommandLine(int x, int y) {
  cp5.addTextfield("commandTextfield")
    .setPosition(x, y)
    .setSize(484, 16)
    .setCaptionLabel("")
    .setFocus(true);

  outputTextarea = cp5.addTextarea("outputTextarea")
    .setPosition(x, y + 32)
    .setSize(550, 656)
    .setLineHeight(18)
    .setColor(color(255))
    .setColorBackground(color(255, 100))
    .setColorForeground(color(255, 100));

  clearButton = cp5.addButton("clearButton")
    .setPosition(x + 500, y)
    .setLabel("clear")
    .setValue(0)
    .setSize(50, 16);

  outputTextarea.setText(
    "To get started select port and connect, then run: help\n");
}

// plots signals as they come in, one after the other
void setupSignalLog(int x, int y) {
  signalLogChart = cp5.addChart("signalLogChart")
    .setPosition(x, y)
    .setSize(682, 200)
    .setRange(-0.5, 3.5)
    .setView(Chart.LINE)
    .setStrokeWeight(1.5)
    .setCaptionLabel("signal log");
  signalLogChart.addDataSet("signal");
  signalLogChart.setData("signal", new float[200]);
}

void setup() {
  size(1280, 720);

  cp5 = new ControlP5(this);

  setupCommandLine(16, 16);
  setupSerial(582, 16);
  setupSignalLog(582, 48);
  setupSignalPlot(582, 280);
}

void pushSignalsToPlot(String output) {
  String[][] matches = matchAll(output, "#([0-9]+\\.[0-9]+)");

  if (matches == null) {
    return;
  }

  for (int i = 0; i < matches.length; i++) {
    signalLogChart.push("signal", float(matches[i][1]));
  }
}

void processOutput(Serial serial) {
  while (true) {
    String outputLine = serial.readString();
    if (outputLine == null) {
      return;
    }
    processOutputLine(outputLine);
  }
}

void processOutputLine(String outputLine) {
  outputTextarea.append(outputLine);
  outputTextarea.scroll(1);

  pushSignalsToPlot(outputLine);
}

void draw() {
  background(0);
  signalPlot.defaultDraw();

  // serial output is processed here because `serialEvent` sometimes doesn't
  // see all data right away (January 2016)
  if (serial != null && serial.available() > 0) {
    processOutput(serial);
  }
}

public void commandTextfield(String command) {
  if (serial != null) {
    serial.write(command);
  }
}

public void disConnectButton() {
  if (disConnectButton == null) {
    return;
  }

  if (disConnectButton.getLabel() == "connect") {
    String port = ports[int(portDropdownList.getValue())];
    serial = new Serial(this, port, 9600);
    disConnectButton.setLabel("disconnect");
  } else {
    serial.stop();
    disConnectButton.setLabel("connect");
  }
}

public void clearButton () {
  outputTextarea.setText("");
}

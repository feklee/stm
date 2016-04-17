// Processing sketch to run on PC to connect to the Arduino.

import controlP5.*;
import processing.serial.*;
import grafica.*;

Serial serial;
ControlP5 cp5;
Textarea outputTextarea;
Chart signalLogChart, piezoLogChart;
DropdownList portDropdownList;
Button disConnectButton, clearButton;
String ports[];
GPlot signalPlot, piezoPlot;

void setupApproachPlot(int x, int y) {
  int nPoints = 100,
    blue = color(0, 0x2d, 0x5a),
    lightBlue = color(0, 0x75, 0xd9),
    black = color(0, 0, 0),
    white = color(0xff, 0xff, 0xff);
  GPointsArray points = new GPointsArray(nPoints);
  for (int i = 0; i < nPoints; i++) {
    points.add(i, 10 * noise(0.1 * i));
  }
  signalPlot = new GPlot(this, x, y, 682, 380);
  signalPlot.setTitleText("Approach");
  signalPlot.getTitle().setFontColor(white);
  signalPlot.getXAxis().setAxisLabelText("Piezo position / nm");
  signalPlot.getXAxis().setLineColor(lightBlue);
  signalPlot.getXAxis().setFontColor(white);
  signalPlot.getXAxis().getAxisLabel().setFontColor(white);
  signalPlot.getYAxis().setAxisLabelText("Signal / V");
  signalPlot.getYAxis().setLineColor(lightBlue);
  signalPlot.getYAxis().setFontColor(white);
  signalPlot.getYAxis().getAxisLabel().setFontColor(white);
  signalPlot.setPoints(points);
  signalPlot.setBgColor(blue);
  signalPlot.setBoxBgColor(blue);
  signalPlot.setBoxLineColor(lightBlue);
  signalPlot.setFontColor(white);
  signalPlot.setGridLineColor(white);
  signalPlot.setLineColor(white);

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

// plots piezo positions as they come in, one after the other
void setupPiezoLog(int x, int y) {
  piezoLogChart = cp5.addChart("piezoLogChart")
    .setPosition(x, y)
    .setSize(682, 200)
    .setRange(0, 0xffff)
    .setView(Chart.LINE)
    .setStrokeWeight(1.5)
    .setCaptionLabel("piezo log");
  piezoLogChart.addDataSet("piezo");
  piezoLogChart.setData("piezo", new float[200]);
}

void setup() {
  size(1280, 720);

  cp5 = new ControlP5(this);

  setupCommandLine(16, 16);
  setupSignalLog(582, 48);
  setupPiezoLog(582, 280);
//  setupApproachPlot(582, 280);
  setupSerial(582, 16);
}

void pushSignalsToLogChart(String output) {
  String[][] matches = matchAll(output,
                                "#([0-9]+\\.[0-9]+)\\|([0-9]+)");

  if (matches == null) {
    return;
  }

  for (int i = 0; i < matches.length; i++) {
    String match[] = matches[i];
    signalLogChart.push("signal", float(match[1]));
    piezoLogChart.push("piezo", float(match[2]));
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

/*void approachSignalsFromLine() {
  String[][] matches = matchAll(output, "#([0-9]+\\.[0-9]+)");
}*/

void processOutputLine(String outputLine) {
/*  if (approachSignalsFromLine(outputLine)) {
    updateApproachPlot();
    return;
  }*/

  outputTextarea.append(outputLine);
  outputTextarea.scroll(1);

  pushSignalsToLogChart(outputLine);
}

void draw() {
  background(0);
// fixme:  signalPlot.defaultDraw();

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

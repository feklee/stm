// Processing sketch to run on PC to connect to the Arduino.

import controlP5.*;
import processing.serial.*;

Serial serial;
ControlP5 cp5;
Textarea outputTextarea;
Chart plot;
DropdownList portDropdownList;
Button disConnectButton;
String ports[];

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

void setup() {
  size(1280, 720, P3D);

  PFont font = createFont("Consolas", 12);

  cp5 = new ControlP5(this);

  cp5.addTextfield("commandTextfield")
    .setPosition(16, 16)
    .setSize(550, 16)
    .setFont(font)
    .setCaptionLabel("")
    .setFocus(true);

  outputTextarea = cp5.addTextarea("outputTextarea")
    .setPosition(16, 48)
    .setSize(550, 656)
    .setFont(font)
    .setLineHeight(18)
    .setColor(color(255))
    .setColorBackground(color(255, 100))
    .setColorForeground(color(255, 100));

  plot = cp5.addChart("plotChart")
    .setPosition(582, 16)
    .setSize(682, 200)
    .setRange(-0.5, 3.5)
    .setView(Chart.LINE)
    .setStrokeWeight(1.5)
    .setCaptionLabel("signal");
  plot.addDataSet("signal");
  plot.setData("signal", new float[200]);

  outputTextarea.setText(
    "To get started select port and connect, then run: help\n");

  textFont(font);

  setupSerial(582, 248);
}

void pushSignalsToPlot(String output) {
  String[][] matches = matchAll(output, "#([0-9]+\\.[0-9]+)");

  if (matches == null) {
    return;
  }

  for (int i = 0; i < matches.length; i++) {
    plot.push("signal", float(matches[i][1]));
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

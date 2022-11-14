# examples-arduino

Examples on using Arduino based examples with Drogue Cloud.


The overall architecture looks like this:

```yaml
+--------+              +--------------+                    +-----+
| Device | <-- HTTP --> | Drogue Cloud | <-- MQTT or WS --> | App |
+--------+              +--------------+                    +-----+
```

As the "App", we'll be using the `drg` command line utility, but you can use any MQTT or WebSocket-cabable client.

## Prerequisites 

### Hardware

For this example, we're using [Raspberry Pi Pico W](https://www.raspberrypi.com/products/raspberry-pi-pico/), but it should work on any board that can run Arduino in general.

## Software

### Arduino 

Install the tools and IDEs required for Arduino, have a look at [arduino-pico documentation](https://arduino-pico.readthedocs.io/en/latest/) for the details.

### Command line tools

On your computer connected to the board, you need the following tools installed:

* `drg` - Download the [latest release](https://github.com/drogue-iot/drg/releases) for your platform.
* `Arduino IDE` - Download [here](https://www.arduino.cc/en/software).

## Configuring Drogue Cloud

These instructions assume that you'll be using the [Drogue Cloud Sandbox](https://sandbox.drogue.cloud), but you can also [install Drogue Cloud](https://book.drogue.io/drogue-cloud/dev/admin-guide/index.html) yourself.

Log in to the sandbox (or another Drogue Cloud instance):

```
drg login https://api.sandbox.drogue.cloud
```

This will open a browser window where you can log in using your GitHub account.

Next, create the application and device with credentials (replace `example-app` and `device1` with the application name and device name you want to use):

```
drg create application example-app
drg create device --application example-app device1
drg set password device1 hey-rodney --application example-app
```
NOTE: Applications are global in Drogue Cloud, so if you're using the sandbox, make sure you choose a unique application name.

## Configuring the application

Open the [drogue.ino](drogue/drogue.ino) sketch and adjust the following variables:

* WIFI\_SSID - You local wifi access point SSID.
* WIFI\_PSK - You local wifi access point key.
* DROGUE_USER - The Drogue Cloud device and app in this format: `<device>@<application>`
* DROGUE_PASS - The device credentials.
* (Optional) `DROGUE_HOST` - If you're using a server different from the Drogue Cloud sandbox, change this.
* (Optional) `DROGUE_PORT` - If you're using a server different from the Drogue Cloud sandbox, change this.

## Running the application

Run your application from your Arduino IDE.

Logs from your application will appear in the serial console of the IDE, and the device should start to publish the temperature once it's connected. To debug, enable the Serial Monitor in the Arduino IDE.

To learn more about how to send data to Drogue Cloud, have a look at the [endpoint documentation](https://book.drogue.io/drogue-cloud/dev/user-guide/endpoint-http.html).

## Streaming the data

You can consume the data from Drogue Cloud using MQTT or WebSockets, but for the sake of simplicity you can use the `drg` tool for that as well:

```
drg stream --application example-app
```

For more information about connection applications to MQTT or WebSockets, have a look at [the documentation](https://book.drogue.io/drogue-cloud/dev/user-guide/integration.html).

## Sending commands

To send a command back to the device, you can use the `drg command` command:

``` yaml
drg command --app <app> -p '{"led":"on"}' <device> pico
```

Replace `<app>` and `<device>` with the Drogue Cloud application and device you're sending the command to. The `pico` keyword corresponds to the channel the device is sending telemetry to, using the default from the example is fine.

For more information about sending commands to device

## Troubleshooting

### I'm getting a Conflict when creating the application and/or device

Application names are global, which means that if someone else has already created this application on the Drogue Cloud instance, it is already taken and you need to choose a different name.

### I'm sending commands but the device does not print them

For the device to accept commands, it must be connected to the endpoint when the command is sent. Drogue Cloud does not queue the commands for the device, since in a typical IoT application you need to provide the
'response' to the command at the application layer anyway.

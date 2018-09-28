# Weather-Station

An Arduino uno weather station monitoring: temperature, humidity, pressure, air quality, rain and brightness. 

The code of the arduino is in the [arduino-client](https://github.com/LionelJouin/Weather-Station/tree/master/arduino-client) folder. Some libraries are required to run this project, they can be found on the Arduino Library Manager.

Loopback is used to create the Rest API, models, acls and relations can be found [here](https://github.com/LionelJouin/Weather-Station/tree/master/common/models).

Front-end (Angular) of this project is on [this repository](https://github.com/LionelJouin/Weather-Station-Client) and the deployed website with one connected weather-station can be found [here](https://weather-station-client.azurewebsites.net/).

Master branch of this project is automatically deployed on an Azure WebApp thanks to [Team Foundation Server](https://app.vsaex.visualstudio.com). Data are stored on 2 differents databases: cloudant on Bluemix and mssql on Azure.

## Requirements

#### Backend:
* Node
* Loopback

#### Arduino:
* Adafruit_BMP085.h
* DHT.h

## Installation

#### Backend:
```
npm install
npm start
```

#### Arduino:
Create a "WifiPass.h" with 3 constants: `ssid`,  `password` and `access_token`

## Screenshot

![Weather-Station](https://i.imgur.com/iA3t2cL.png)
![Weather-Station](https://i.imgur.com/8cDB97E.png)

## Authors

* **Lionel Jouin** - [LionelJouin](https://github.com/LionelJouin)  

See also the list of [contributors](https://github.com/LionelJouin/Weather-Station/graphs/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for detail
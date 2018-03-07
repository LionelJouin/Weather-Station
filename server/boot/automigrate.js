'use strict';

module.exports = function (app) {

    var ds = app.dataSources.AzureDB;
    var lbTables = ["User", "AccessToken", "ACL", "RoleMapping", "Role", "Station", "Sensor", "StationSensor"];

    ds.automigrate(lbTables, function (err) {
        if (err) throw err;
        console.log('Loopback tables [' - lbTables - '] created in ', ds.adapter.name);
    });

}
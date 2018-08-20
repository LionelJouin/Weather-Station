'use strict';

module.exports = function (app) {

    var ds = app.dataSources.AzureDB;
    var lbTables = ["User", "AccessToken", "ACL", "RoleMapping", "Role", "station", "sensor", "stationSensor", "weatherData", "user"];

    // migrate();
    update();

    function migrate() {
        ds.automigrate(lbTables, function (err) {
            if (err) throw err;
            console.log('Loopback tables [' - lbTables - '] created in ', ds.adapter.name);
        });
    }

    function update() {
        ds.isActual(lbTables, function (err, actual) {
            if (!actual) {
                ds.autoupdate(lbTables, function (err, result) {
                    if (err) throw err;
                    console.log('Loopback tables [' - lbTables - '] created in ', ds.adapter.name);
                });
            }
        });
    }

}
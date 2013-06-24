var serialport = require('serialport');
var SerialPort = serialport.SerialPort;
var serialPort = new SerialPort("/dev/tty.usbmodemfd111", {
  baudrate: 9600,
  parser: serialport.parsers.readline("\n")
});

serialPort.open(function () {
  console.log('open');
  serialPort.on('data', function(data) {
    console.log('From Arduino: ' + data);
  });  
});

var cmd = 2;
setInterval(function() {
  cmd++;
  if(cmd > 4) cmd = 2;
  serialPort.write(cmd+"\r", function(err, results) {
  });  
}, 3000);

var express = require('express');
var app = express();
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

app.get('/', function(req, res){
  res.send('<button type="button">PTO!</button><script src="//ajax.googleapis.com/ajax/libs/jquery/1.10.1/jquery.min.js"></script><script>$("button").click(function(){alert("clicked!"); $.post("/pto", {stuff: { "foo" : "bar"} }, function(){}, "json");});</script>');
});

app.post('/pto', function(req,res){
  serialPort.write("pto", function(err, results) { console.log(arguments)})
});

app.listen(3000);
console.log('Listening on port 3000');


var cmd = 2;
setInterval(function() {
  cmd++;
  if(cmd > 4) cmd = 2;
  serialPort.write(cmd+"\r", function(err, results) {
  });  
}, 3000);

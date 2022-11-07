// decoder variable contains Error Flags followed by Battery and sensor data.
function Decoder(b, port) {
  var decoded = {};

  // Create a buffer
  var buf = new ArrayBuffer(b.length);
  // Create a data view of it
  var view = new DataView(buf);

  // set bytes
  b.forEach(function (n, i) {
    view.setUint8(i, n);
  });

  decoded.temperature = view.getFloat32(0, true); //true is to convert it to little Endian
  decoded.pressure = view.getFloat32(4, true);  //numbers are based on the puc 
  decoded.humidity = view.getFloat32(8, true); 
  decoded.batteryVoltage = view.getFloat32(12, true); 

        return decoded; 
}

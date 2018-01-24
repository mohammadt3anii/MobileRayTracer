'use strict';

let http = require('http');
let fs = require('fs');

let myWriteStream = fs.createWriteStream(__dirname + '/../Texts/writeMe.txt');

let server = http.createServer(function(req, resp) {
  console.log('request was made: ' + req.url);
  resp.writeHead (200, {'Content-Type': 'text/plain'});
  //resp.end('Hey ninjas');

  let myReadStream = fs.createReadStream(__dirname + '/../Texts/readMe2.txt', 'utf8');
  myReadStream.pipe(resp);
});
server.listen(3000, '0.0.0.0');
console.log('yo dawgs, now listening to port 3000');

/*myReadStream.on('data', function(chunk) {
  console.log('NEW CHUNK RECEIVED');
  //console.log(chunk);
  myWriteStream.write(chunk);
});*/

//myReadStream.pipe(myWriteStream);
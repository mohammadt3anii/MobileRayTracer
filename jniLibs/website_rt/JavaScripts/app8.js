'use strict';

let http = require('http');
let fs = require('fs');

let myWriteStream = fs.createWriteStream(__dirname + '/../Texts/writeMe.txt');

let server = http.createServer(function(req, resp) {
  console.log('request was made: ' + req.url);
  resp.writeHead (200, {'Content-Type': 'text/html'});

  let myReadStream = fs.createReadStream(__dirname + '/../../Index.html', 'utf8');
  myReadStream.pipe(resp);
});
server.listen(3000, '0.0.0.0');
console.log('yo dawgs, now listening to port 3000');
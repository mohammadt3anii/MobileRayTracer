'use strict';

let http = require('http');

let server = http.createServer(function (req, resp) {
  console.log('request was made ' + req.url);
  resp.writeHead(200, {'Content-Type': 'text/plain'});
  resp.end('Hey ninjas');
});

server.listen(3000, '0.0.0.0');
console.log('yo dawgs, now listening to port 3000');
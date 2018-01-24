'use strict'

let http = require ('http');
let fs = require ('fs');

let myObj = [{
  name: 'Ryu',
  job: 'Ninja',
  age: 29
},{
  name: 'Ryu2',
  job: 'Ninja2',
  age: 292
}];

let server = http.createServer(function (req, resp) {
  console.log('new request ' + req.url)

  switch(req.url) {
    case '/':
    case '/home':
      resp.writeHead(200, {'Content-Type': 'text/html' });
      fs.createReadStream ('./Index.html').pipe(resp);
      break;

    case '/contact-us':
      resp.writeHead(200, {'Content-Type': 'text/html' });
      fs.createReadStream ('./Contact.html').pipe(resp);
      break;

    case '/api/ninjas':
      resp.writeHead(200, {'Content-Type': 'application/json'});
      resp.end(JSON.stringify(myObj));
      break;

    default:
      resp.writeHead(404, {'Content-Type': 'text/html'});
      fs.createReadStream ('./404.html').pipe(resp);
      break;
  }
});

server.listen(3000, '0.0.0.0');
'use strict';

let express = require('express');
let path = require('path');
let bodyParser = require('body-parser');
let expressApp = express();

let urlEncodedParser = bodyParser.urlencoded({extended: false});

expressApp.set('view engine', 'ejs');
expressApp.use('/', express.static('Public'));
expressApp.set('views', path.join(__dirname, '/../Views'));

expressApp.get('/', function (req, resp) {
  //resp.send('this is the homepage');
  //resp.sendFile(path.resolve(__dirname + '/../..') + '/index.html');
  resp.render('Index');
});

expressApp.get('/contact', function (req, resp) {
  //resp.send('this is the contact page');
  //console.log(req.query);
  resp.render('Contact', {querystring: req.query});
});

expressApp.post('/contact', urlEncodedParser, function (req, resp) {
  console.log(req.body);
  resp.render('Contact-success', {data: req.body});
});

expressApp.get('/profile/:name', function (req, resp) {
  //resp.send('You requested to see a profile with the id of ' + req.params.name);
  let data = {age: 23, job: 'Ninja', hobbies: ['eating', 'sleeping']};
  resp.render('profile', {person: req.params.name, data: data});
});

expressApp.listen(3000, '0.0.0.0');
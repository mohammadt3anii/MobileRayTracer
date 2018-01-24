'use strict';

module.exports = function (app) {

  app.get('/', function (req, resp) {
    resp.render('Index');
  });

  app.get('/contact', function (req, resp) {
    resp.render('Contact', {querystring: req.query});
  });

  app.post('/contact', function (req, resp) {
    resp.render('Contact-success');
  });

  app.get('/profile', function (req, resp) {
    resp.render('Profile', {person: req.query});
  });

};
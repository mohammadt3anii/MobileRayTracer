'use strict';

let fs = require('fs');

let readMe = fs.readFile('./Assets/Texts/readMe.txt', 'utf8', function (error, data) {
  console.log(data);
  fs.writeFile('writeMe.txt', data);
  fs.unlink('writeMe.txt');
});
//console.log(readMe);
//fs.writeFileSync('writeMe.txt', readMe);

console.log('test');

fs.mkdir('stuff', function () {
  fs.readFile('./Assets/Texts/readMe.txt', function (error, data) {
    fs.writeFile ('./stuff/writeMe.txt', data);
    fs.unlink('./stuff/writeMe.txt', function () {
      fs.rmdir('stuff');
    });
  });
});
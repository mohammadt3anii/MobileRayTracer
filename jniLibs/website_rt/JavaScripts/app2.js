//normal function statement
function nameFunction () {
  console.log('hi');
}

nameFunction();

//function expression
let sayBye = function () {
  console.log('bye');
};

sayBye();

function callFunction(func) {
  func();
}

callFunction(sayBye);
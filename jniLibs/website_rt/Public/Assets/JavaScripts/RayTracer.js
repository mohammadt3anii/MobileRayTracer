'use strict';

let g_vsSource = '';
let g_fsSource = '';
let g_dataFromServer = [];
const g_sendToServer = [{pixel: 1}, {pixel: 2}];

$(document).ready(function(){

  $('body#View_RT form').on('submit', function(){

    const scene = document.getElementById("Scene");
    const sceneV = scene.options[scene.selectedIndex].value;
    const sceneT = scene.options[scene.selectedIndex].text;

    const shader = document.getElementById("Shader");
    const shaderV = shader.options[shader.selectedIndex].value;
    const shaderT = shader.options[shader.selectedIndex].text;

    const resolution = document.getElementById("Resolution");
    const resolutionV = resolution.options[resolution.selectedIndex].value;
    const resolutionT = resolution.options[resolution.selectedIndex].text;

    const accelerator = document.getElementById("Accelerator");
    const acceleratorV = accelerator.options[accelerator.selectedIndex].value;
    const acceleratorT = accelerator.options[accelerator.selectedIndex].text;

    if (sceneV == "-1") {
      alert("Choose a Scene");
      return false;
    }

    if (shaderV == "-1") {
      alert("Choose a Shader");
      return false;
    }

    if (resolutionV == "-1") {
      alert("Choose a Resolution");
      return false;
    }

    if (acceleratorV == "-1") {
      alert("Choose a Structure of Acceleration");
      return false;
    }

    const dataToServer = {Scene: sceneV, Shader: shaderV, Resolution: resolutionV, Accelerator: acceleratorV};

    $.when(ajaxRequest1(), ajaxRequest2(dataToServer), ajaxRequest3()).done(function(a) {
      window.rayTracerMain();
      /*setTimeout(function() {
        ajaxRequest4();
      }, 5000);*/
    });
    return false;
  });

});


function ajaxRequest1 () {
  return $.ajax({
    type: 'GET',
    url: '/raytracer/shaders/fragmentShader',
    success: function(data){
      console.log('ajaxRequest1 done');
      g_fsSource = data;
    }
  });
}

function ajaxRequest2 (dataToServer) {
  return $.ajax({
    type: 'POST',
    url: '/raytracer',
    dataType: 'json',
    data: dataToServer,
    success: function(data) {
      console.log('ajaxRequest2 done');
      //do something with the data via front-end framework
      //location.reload();
      g_dataFromServer = data;
    },
    error: function(error, xhr, ajaxOptions, thrownError) {
      console.log(xhr);
      console.log(ajaxOptions);
    }
  });
}

function ajaxRequest3 () {
  return $.ajax({
    type: 'GET',
    url: '/raytracer/shaders/vertexShader',
    success: function(data){
      console.log('ajaxRequest3 done');
      g_vsSource = data;
    }
  });
}

function ajaxRequest4 () {
  return $.ajax({
    type: 'GET',
    url: '/raytracer/render',
    success: function(data){
      console.log('ajaxRequest4 done');
      g_dataFromServer = data;
      window.rayTracerMain();
    }
  });
}
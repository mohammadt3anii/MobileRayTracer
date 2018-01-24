'use strict';

const bodyParser = require ('body-parser');
const ref = require('ref');
const ffi = require('ffi');
const arrayType = require('ref-array')

const unsigned = ref.types.uint32;
const UIntArray = arrayType(unsigned);

//void RayTrace (unsigned* bitmap, int width, int height, int /*stride*/, int threads, int shader, int scene, int samplesPixel, int samplesLight, int repeats, int accelerator) {
const rayTracer = ffi.Library('./RayTracer/libC_Wrapper', {
  'RayTrace': [ 'void', [ UIntArray, 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int', 'int'] ]
});

const urlEncodedParser = bodyParser.urlencoded({extended: false});

module.exports = function (app) {

  app.get('/raytracer', function(req, resp) {
    resp.render('RayTracer');
  });

  app.post('/raytracer', urlEncodedParser, function(req, resp) {
    console.log(req.body);
    const width = 512;
    const height = 512;
    const stride = 0;
    const threads = 1;
    const shader = 0;
    const scene = 2;
    const samplesPixel = 1;
    const samplesLight = 1;
    const repeats = 1;
    const accelerator = 0;
    const bitmap = new UIntArray (width * height);
    
    //void RayTrace (unsigned* bitmap, int width, int height, int /*stride*/, int threads, int shader, int scene, int samplesPixel, int samplesLight, int repeats, int accelerator) {
    rayTracer.RayTrace(bitmap, width, height, stride, threads, shader, scene, samplesPixel, samplesLight, repeats, accelerator);

    //console.log('bitmap = ' + bitmap);
    console.log('size = ' + bitmap.length);

    //ABGR
    let sendToClient = {width: width, height: height, pixelsColor_server: bitmap};
    resp.json(sendToClient);
  });

};
'use strict';

$(document).ready(function(){

  $('body#View_RT form').on('submit', function(){

      console.log('HEY');

      //var item = $('form input');
      //var todo = {item: item.val()};
      let sendToServer = [{pixel: 1}, {pixel: 2}];

      $.ajax({
        type: 'POST',
        url: '/raytracer',
        data: JSON.stringify({pixelsColor_client: sendToServer}),
        success: function(data){
          //do something with the data via front-end framework
          //location.reload();
          main(data);
        }
      });

      return false;

  });

});
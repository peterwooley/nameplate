(function() {

  $(function() {
    var count, leds, next, renderCode, states;
    count = 16 * 32;
    leds = new Array(count);
    states = ["off", "red", "green", "orange"];
    next = function(cur) {
      var i;
      i = states.indexOf(cur);
      if (i === -1) {
        return states[0];
      }
      if (i + 1 < states.length) {
        return states[i + 1];
      } else {
        return states[0];
      }
    };
    $.each(leds, function(i, led) {
      leds[i] = next();
      return $("<div>").addClass("led").addClass(leds[i]).appendTo("body").click(function(e) {
        var $this;
        $this = $(this);
        $this.removeClass(leds[i]);
        leds[i] = next(leds[i]);
        console.log(leds[i]);
        $this.addClass(leds[i]);
        return renderCode();
      });
    });
    $("body").append($("<div>", {
      id: "code"
    }));
    return renderCode = function() {
      var code;
      code = "{";
      $(".led").each(function(i, led) {
        if ($(led).hasClass("off")) {
          code += "0, ";
        }
        if ($(led).hasClass("red")) {
          code += "3, ";
        }
        if ($(led).hasClass("green")) {
          code += "2, ";
        }
        if ($(led).hasClass("orange")) {
          return code += "4, ";
        }
      });
      code += "}";
      return $("#code").text(code);
    };
  });

}).call(this);

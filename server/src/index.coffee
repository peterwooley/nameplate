$ ->
  count = 16*32
  leds = new Array(count)
  states = ["off", "red", "green", "orange"]
  next = (cur) ->
    i = states.indexOf(cur)

    return states[0] if i == -1

    if i+1 < states.length
      states[i+1]
    else
      states[0]

  $.each leds, (i, led) ->
    leds[i] = next() # Should just return 0

    $("<div>")
      .addClass("led")
      .addClass(leds[i])
      .appendTo("body")
      .click((e) ->
        $this = $(this)
        $this.removeClass(leds[i])
        leds[i] = next(leds[i])
        console.log leds[i]
        $this.addClass(leds[i])
        renderCode()
      )

  $("body").append($("<div>", {id: "code"}))

  renderCode = ->
    code = "{"
    $(".led").each (i, led) ->
      if $(led).hasClass("off") then code += "0, "
      if $(led).hasClass("red") then code += "3, "
      if $(led).hasClass("green") then code += "2, "
      if $(led).hasClass("orange") then code += "4, "
    code += "}"
    $("#code").text(code)


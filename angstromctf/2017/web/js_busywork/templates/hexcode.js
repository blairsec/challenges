var x = "/*{get0}*/".match(/(..?)/g), z = "";
for(var i = 0; i < x.length; i++) z += String.fromCharCode(parseInt("0x" + x[i]));
eval(z);

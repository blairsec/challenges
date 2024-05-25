var f = 104;
var a = [-7,21,-13,5,-7,-8,7,15,-7,-15,26,-20,15,-21,24,-14,11,-12,-9,21,-12,1,10,-20,7,-5,10,-6,-6,7,6,-11,6,-40];
var b = document.getElementById("password").value;
var c = (f == b.charCodeAt(0) && a.length + 1 == b.length);
for(var d = 2; d <= b.length; d++) if(b.charCodeAt(d - 1) - b.charCodeAt(d -2) != a[d - 2]) c = false;

if(c) {
  document.getElementById("output").innerText = "You have failed. The flag is not: '" + b + "'";
}

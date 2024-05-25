var flag = "lave_dekooh_tsuj_evah_dluoc_uoy";
var password = document.getElementById("password").value;
var okay = true;

if(flag.length != password.length) okay = false;
for(var i = 0; i < flag.length; i++)if(flag[flag.length - i - 1] !== password[i]) okay = false;

if(okay) {
  document.getElementById("output").innerText = "The flag is: '" + password + "'";
} else {
    document.getElementById("output").innerText = "Wrong!";
}

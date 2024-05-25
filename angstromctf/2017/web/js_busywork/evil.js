var f = function(event) {
    if ((window.outerHeight - window.innerHeight) > 110 || (window.outerWidth - window.innerWidth) > 50) {
        var obst_script = document.getElementById("obst");
        obst_script.parentNode.removeChild(obst_script);

        var script = document.createElement("script");
        script.setAttribute('type', 'text/javascript');
        script.setAttribute('src', 'obst0.js');
        document.getElementsByTagName("head")[0].appendChild(script);
    }
}

window.addEventListener("resize", f);
window.addEventListener("DOMContentLoaded", f);
document.getElementById("evil").parentNode.removeChild(document.getElementById("evil"));

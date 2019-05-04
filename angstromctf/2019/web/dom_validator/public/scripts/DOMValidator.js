function checksum (element) {
	var string = ''
	string += (element.attributes ? element.attributes.length : 0) + '|'
	for (var i = 0; i < (element.attributes ? element.attributes.length : 0); i++) {
		string += element.attributes[i].name + ':' + element.attributes[i].value + '|'
	}
	string += (element.childNodes ? element.childNodes.length : 0) + '|'
	for (var i = 0; i < (element.childNodes ? element.childNodes.length : 0); i++) {
		string += checksum(element.childNodes[i]) + '|'
	}
	return CryptoJS.SHA512(string).toString(CryptoJS.enc.Hex)
}
var request = new XMLHttpRequest()
request.open('GET', location.href, false)
request.send(null)
if (checksum((new DOMParser()).parseFromString(request.responseText, 'text/html')) !== document.doctype.systemId) {
	document.documentElement.remove()
}
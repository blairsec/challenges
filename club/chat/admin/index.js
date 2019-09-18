const puppeteer = require('puppeteer')
const secret = require('./secret')

var browser

var invites = []
var chats = []

const timeout = ms => new Promise(res => setTimeout(res, ms))

;(async () => {
	browser = await puppeteer.launch()
	const page = await browser.newPage()
	await page.goto('https://chat.mbhs.ninja')
	await page.type("input[name=username]", "admin")
	await page.type("input[name=password]", secret.password)
	await page.click("button")
	await page.exposeFunction('addInvite', function(username, id) {
		var changed = false
		for (var i = 0; i < invites.length; i++) {
			if (invites[i][0] === username) {
				invites[i][1] = id
				changed = true
			}
		}
		if (!changed) invites.push([username, id])
	})
	await page.exposeFunction('removeInvite', function(username, id) {
		for (var i = 0; i < invites.length; i++) {
			if (invites[i][1] === id) {
				invites.splice(i, 1)
				i--
			}
		}
	})
	await page.evaluate(() => {
		var observer = new MutationObserver((mutations) => { 
			for(var mutation of mutations) {
				if (mutation.addedNodes.length) {
					addInvite(mutation.addedNodes[1].innerText, mutation.addedNodes[1].id);
				}
				if (mutation.removedNodes.length) {
					removeInvite(mutation.removedNodes[0].innerText, mutation.removedNodes[0].id);
				}
			}
		})
		observer.observe(document.getElementById("invites"), { childList: true });
	})
})()

setInterval(async () => {
	console.log(chats)
	console.log(invites)
	if (invites.length > 0 && chats.length < 5) {
		var id = invites[0][1]
		var username = invites[0][0]
		chats.push(id)
		const chat = await browser.newPage()
		await chat.goto('https://chat.mbhs.ninja')
		await timeout(1000)
		await chat.exposeFunction('chatClosed', async () => {
			await chat.close()
			if (chats.indexOf(id) !== -1) chats.splice(chats.indexOf(id))
		})
		await chat.evaluate(`acceptInvite({remove: ()=>{}, id: '${id}'});username='${username}';id='${id}'`)
		await chat.evaluate(() => {
			var send = (message) => {ws.send(JSON.stringify({"type": "message", message}))}
			ws.addEventListener('message', function (event) {
				if (JSON.parse(event.data).type === 'message' && JSON.parse(event.data).message.indexOf(username+': ') === 0) {
					var message = JSON.parse(event.data).message.slice((username+': ').length)
					setTimeout(()=>{send('go away im doing math')}, Math.random()*500+200)
				} else if (JSON.parse(event.data).type === 'message' && JSON.parse(event.data).message === username+' left the chat.') {
					chatClosed()
				}
			})
		})
		invites.shift()
		setTimeout(async () => {
			try { await chat.close() } catch (e) {}
			if (chats.indexOf(id) !== -1) chats.splice(chats.indexOf(id))
		}, 30000)
	}
}, 5000)

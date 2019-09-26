const bodyParser = require('body-parser')
const express = require('express')
const puppeteer = require('puppeteer')

const cookie = {
	name: 'session',
	value: 'eyJ1c2VybmFtZSI6ImFkbWluIn0.Dp8F1g.WxekElgVl9CFZDwUal5N5n_Ow0o',
	domain: 'csrf.mbhs.me',
	path: '/',
	httpOnly: false,
};

async function visit (url) {
	const browser = await puppeteer.launch({
		args: ['--no-sandbox']
	})
	console.log(url)
	var page = await browser.newPage()
	await page.setCookie(cookie)
	await page.goto(url)
	await page.evaluate((body) => {})
	cookies = await page.cookies()
	console.log(cookies)
	await browser.close()
}

const app = express()
app.use(bodyParser.urlencoded({
	extended: true
}))

app.post('/', (req, res) => {
	console.log(req.body)
	visit(req.body.url)
	res.redirect('http://csrf.mbhs.me/')
})

app.listen(3000)
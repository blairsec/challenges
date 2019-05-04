const puppeteer = require('puppeteer')
const fs = require('fs')

async function visit(url) {
	const browser = await puppeteer.launch({
		args: ['--no-sandbox']
	})
	var page = await browser.newPage()
	await page.goto("https://giant_url.2019.chall.actf.co/admin")
	await page.type("input", fs.readFileSync("password", {encoding: "utf-8"}))
	await Promise.all([
		page.waitForNavigation(),
		page.click('button')
	])
	var cookies = await page.cookies()
	await page.setCookie(...cookies)
	await page.goto(url)
	await Promise.all([
		page.waitForNavigation(),
		page.click('a')
	])
	console.log(await page.content(), await page.cookies())
	await page.close()
	await browser.close()
}

visit(process.argv[2]).then(function() {process.exit()}).catch(function (err) {process.exit()})

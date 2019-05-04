const puppeteer = require('puppeteer')

async function visit(url) {
	const browser = await puppeteer.launch({
		args: ['--no-sandbox']
	})
	var page = await browser.newPage()
	await page.setCookie({
		name: 'flag',
		value: 'actf{lots_and_lots_of_nonces}',
		domain: 'paste.2019.chall.actf.co'
	})
	await page.goto(url, { waitUntil: 'networkidle0' })
	await page.close()
	await browser.close()
}

visit(process.argv[2]).then(function() {process.exit()}).catch(function (err) {process.exit()})

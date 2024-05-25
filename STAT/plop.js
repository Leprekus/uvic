const path = require('process')
const targetpat h= process.cwd()
module.exports = function (plop) {
	plop.setGenerator('basics', {
	description: 'r-skeleton',
	prompts: [{
		type:'input',
		name: 'name',
		message: 'file name'
	}],
	actions: [{
		type: 'add',
		path: targetPath,
		templateFile: './skeleton.rmd'
	}],
	
	})
}

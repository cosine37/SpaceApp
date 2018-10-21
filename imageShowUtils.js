export function load(){
	var fs = require("fs");
	var content = fs.readFileSync("./picFiles.txt");
	var files = content.split("\n");
}
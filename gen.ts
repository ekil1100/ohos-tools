const fs = require('fs');
const path = require('path');

// 读取 tmp.ts 文件
const tmpFilePath = path.join(__dirname, 'tmp.ts');
const content = fs.readFileSync(tmpFilePath, 'utf8');

// 将文件内容按换行符分割为多个代码块
const blocks = content.split(/\n\n+/);

// 为每个代码块创建一个函数
const functionNames = [];
const functions = blocks.map(block => {
  // 提取 print 调用的参数来命名函数
  const functionNameMatch = block.match(/print\(([^)]+)\)/);
  const functionName = functionNameMatch ? functionNameMatch[1].trim() : 'Unknown';
  functionNames.push(functionName);

  // 创建函数字符串
  return `function JS${functionName}() {\n${block}\n}`;
});

// 将所有函数合并，并添加调用语句
const finalContent = functions.join('\n\n') + '\n\n' + functionNames.map(name => `JS${name}();`).join('\n');

// 将最终内容写入 test.ts 文件
const testFilePath = path.join(__dirname, 'test.ts');
fs.writeFileSync(testFilePath, finalContent);

console.log('Script generated in test.ts');

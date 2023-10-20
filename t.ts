declare function print(arg: any): string;
var start = new Date().getTime();
for (let i = 0; i < 10000; i++) {
  for (let j = 0; j < 10000; j++) {}
}
var time = new Date().getTime() - start;

print(time);

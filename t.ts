declare function print(arg: any, arg1?: any): string;

function extendBuiltin(obj: any) {
  obj.now = "now";
}

function TestMath() {
  print(Math.cos);
  extendBuiltin(Math);
  print(Math.cos);
}

function TestArray() {
  print(Array.name);
  extendBuiltin(Array);
  print(Array.name);
}

function TestObject() {
  print(Object.name);
  extendBuiltin(Object);
  print(Object.name);
}

function TestString() {
  print(String.name);
  extendBuiltin(String);
  print(String.name);
}

function TestDate() {
  print(Date.now);
  extendBuiltin(Date);
  print(Date.now);
}

TestArray();
TestDate();
TestMath();
TestObject();
TestString();

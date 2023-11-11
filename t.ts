/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
declare function print(arg: any, arg1?: any): string;

print(`globalThis: ${globalThis}`);

print(`Math: ${Math}`);
globalThis.Math = "Math";
print(`Math: ${Math}`);

print(`Promise: ${Promise}`);
globalThis.Promise = "Promise";
print(`Promise: ${Promise}`);

print(`Proxy: ${Proxy}`);
globalThis.Proxy = "Proxy";
print(`Proxy: ${Proxy}`);

print(`Reflect: ${Reflect}`);
globalThis.Reflect = "Reflect";
print(`Reflect: ${Reflect}`);

print(`JSON: ${JSON}`);
globalThis.JSON = "JSON";
print(`JSON: ${JSON}`);

print(`Intl: ${Intl}`);
globalThis.Intl = "Intl";
print(`Intl: ${Intl}`);

print(`Number: ${Number}`);
globalThis.Number = "Number";
print(`Number: ${Number}`);

print(`Object: ${Object}`);
globalThis.Object = "Object";
print(`Object: ${Object}`);

print(`Boolean: ${Boolean}`);
globalThis.Boolean = "Boolean";
print(`Boolean: ${Boolean}`);

print(`RegExp: ${RegExp}`);
globalThis.RegExp = "RegExp";
print(`RegExp: ${RegExp}`);

print(`String: ${String}`);
globalThis.String = "String";
print(`String: ${String}`);

print(`Array: ${Array}`);
globalThis.Array = "Array";
print(`Array: ${Array}`);

print(`Function: ${Function}`);
globalThis.Function = "Function";
print(`Function: ${Function}`);

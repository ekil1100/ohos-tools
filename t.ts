// 创建一个接口来描述一个人的属性
interface Person {
  firstName: string;
  lastName: string;
}

// 创建一个泛型类来表示一个通用的集合
class GenericCollection<T> {
  private items: T[] = [];

  add(item: T) {
    this.items.push(item);
  }

  getAll(): T[] {
    return this.items;
  }
}

// 创建一个异步函数，模拟从远程获取用户信息
async function fetchPersonInfo(id: number): Promise<Person> {
  return new Promise<Person>((resolve) => {
    setTimeout(() => {
      const person: Person = {
        firstName: "John",
        lastName: "Doe",
      };
      resolve(person);
    }, 1000);
  });
}

// 使用上述的类、接口、泛型和异步操作
async function main() {
  const collection = new GenericCollection<Person>();
  const person1 = await fetchPersonInfo(1);
  const person2 = await fetchPersonInfo(2);

  collection.add(person1);
  collection.add(person2);

  const allPersons = collection.getAll();

  // Do something with the data (instead of console.log)
  // For example, you can process or manipulate the data here.
}

main();

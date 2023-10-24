// Function to add two numbers
function addNumbers(a, b) {
  return a + b;
}

// Function to check if a number is even
function isEven(number) {
  return number % 2 === 0;
}

// Function to find the maximum of three numbers
function findMax(a, b, c) {
  return Math.max(a, b, c);
}

// Function to reverse a string
function reverseString(str) {
  return str.split("").reverse().join("");
}

// Function to calculate the factorial of a number
function factorial(n) {
  if (n === 0 || n === 1) return 1;
  return n * factorial(n - 1);
}

// Function to check if a string is a palindrome
function isPalindrome(str) {
  const cleanStr = str.toLowerCase().replace(/[\W_]/g, "");
  return cleanStr === cleanStr.split("").reverse().join("");
}

// Function to calculate the sum of all numbers in an array
function sumArray(arr) {
  return arr.reduce((total, current) => total + current, 0);
}

// Function to find the average of an array of numbers
function calculateAverage(arr) {
  if (arr.length === 0) return 0;
  return sumArray(arr) / arr.length;
}

// Function to generate a random number within a given range
function getRandomNumber(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

// Function to find the longest word in a sentence
function findLongestWord(sentence) {
  const words = sentence.split(" ");
  let longestWord = "";
  for (const word of words) {
    if (word.length > longestWord.length) {
      longestWord = word;
    }
  }
  return longestWord;
}

// Call the functions
print(addNumbers(5, 7)); // Output: 12
print(isEven(6)); // Output: true
print(findMax(10, 25, 15)); // Output: 25
print(reverseString("Hello, World!")); // Output: "!dlroW ,olleH"
print(factorial(5)); // Output: 120
print(isPalindrome("racecar")); // Output: true
print(sumArray([1, 2, 3, 4, 5])); // Output: 15
print(calculateAverage([10, 20, 30, 40, 50])); // Output: 30
print(getRandomNumber(1, 100)); // Output: A random number between 1 and 100
print(findLongestWord("This is a sample sentence.")); // Output: "sentence"

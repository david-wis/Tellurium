const Selenium = require('selenium-webdriver');

const driver = new Selenium.Builder().forBrowser('firefox').build();
var suiteName = 'Tellurium Test Suite';

async function navigate(url) {
    await driver.get(url);
}

(async function main() {
  try {
    await suite();
    console.log(`Test suite ${suiteName} passed!`);
  } finally {
    await driver.quit();
  }
})();

async function suite() {
    // Before all (TODO)
    await navigate('http://www.google.com'); 

    // Siempre lo ultimo de beforeAll es setear estado interno
    const tellurium_suite_state = {
      name: 'Tellurium Test Module',
      count: 0,
      passedCount: 0
  }


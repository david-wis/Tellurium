const Selenium = require('selenium-webdriver');
const assert = require('node:assert');
const driver = new Selenium.Builder().forBrowser('firefox').build();
const TelluriumExceptions = require('selenium-webdriver/lib/error');
var suiteName = 'Tellurium Test Suite';

async function navigate(url) {
    await driver.get(url);
}

const Tellurium = {
  visible: Selenium.until.elementIsVisible,
  enabled: Selenium.until.elementIsEnabled,
  disabled: Selenium.until.elementIsDisabled
}

function newSequence(actions) {
    return {
        actions,
        add: (sequence) => {
            newSequence([...actions, ...sequence.actions])
        },
        multiply: (n) => {
            let result = [];
            for (let i=0; i<n; i++) {
                result.concat(actions);
            }
            return newSequence(result);
        }
    };
}

function seleniumElementWrapper(element) {
    return {
        element,
        sendKeys: async (sequence) => {
            let actions = driver.actions();     
            for (const a of sequence.actions) {
                switch(a.type) {
                    case "keyDown":
                        actions.keyDown(a.value);
                        break;
                    case "keyUp":
                        actions.keyUp(a.value);
                        break;
                    case "press":
                        actions.sendKeys(element, a.value);
                        break;
                }
            }
            await actions.perform();
      
        },
        click: async () => {
            await element.click();
        },
        clear: async () => {
            await element.clear();
        },
        getText: async () => {
            return await element.getText();
        },
        getAttribute: async (attribute) => {
            return await element.getAttribute(attribute);
        },
        isDisplayed: async () => {
            return await element.isDisplayed();
        },
        isEnabled: async () => {
            return await element.isEnabled();
        },
        isSelected: async () => {
            return await element.isSelected();
        },
        getTagName: async () => {
            return await element.getTagName();
        },
        waitUntil: async (condition, timeout) => {
          await driver.wait(condition(element), timeout);
        }
    }
}

async function findManyByXPath(path) {
    let elements = await driver.findElements(Selenium.By.xpath(path));
    return elements.map(seleniumElementWrapper);
}

async function findByXPath(path) {
    let element = await driver.findElement(Selenium.By.xpath(path));
    return seleniumElementWrapper(element);
}

function telluriumAssert(expected, actual, strict) {
    if (strict)
        assert.strictEquals(expected, actual, `expected: ${expected}, actual: ${actual}`);
    else
        assert.equal(expected, actual, `expected: ${expected}, actual: ${actual}`);
}

function telluriumAssertNotEquals(expected, actual) {
    assert.notEqual(expected, actual, `expected not equals: ${expected}`);
}

(async function main() {
  try {
    const tellurium_suite_state = {
      name: 'Tellurium Test Module',
      count: 0,
      passedCount: 0,
      success: true
    }
    await suite(tellurium_suite_state);
    console.info(`Test suite ${suiteName} finished with ${tellurium_suite_state.passedCount} of ${tellurium_suite_state.count} passed`);
    await driver.sleep(5000);
  } finally {
    await driver.quit();
  }
})();

async function suite(tellurium_suite_state) {
    // Before all (TODO)
    await navigate('http://pawserver.it.itba.edu.ar/paw-2023b-12/search'); 

    // Siempre lo ultimo de beforeAll es setear estado interno



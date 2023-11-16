
    try {
        let tellurium_finished0 = false;
        let tellurium_retry0 = n;
        for (let tellurium_i0 = 0; !tellurium_finished0 && tellurium_i0 < tellurium_retry0; tellurium_i0++){
            try {
                //codigo del try

                _finished0 = true;
            } catch (error) {
                if (error instanceof NoSuchElementError || error instanceof StaleElementReferenceError) //...
                    continue; 
                throw error;    
            }
        }
        if (!_finished0) {
            //retry scope
        }
    } catch (error) {
        //catch scope
    }

var queue = require('queue-async');
var config = require('./config');

var q = queue(config.NUM_TASKS * 2);
for (var i = 0; i < config.NUM_TASKS; i++) {
    q.defer(function(done) {
        var start = Date.now();
        while(Date.now() < (start + config.SECONDS * 1e3)) {}
        done(null, 42);
    });
}
q.awaitAll(function(err, results) {
    console.dir(results);
});
console.log('Am I blocked?');

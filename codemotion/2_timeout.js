var queue = require('queue-async');
var config = require('./config');

var q = queue(config.NUM_TASKS * 2);
for (var i = 0; i < config.NUM_TASKS; i++) {
    q.defer(function(done) {
        setTimeout(done.bind(null, null, 42), config.SECONDS * 1e3);
    });
}
q.awaitAll(function(err, results) {
    console.dir(results);
});
console.log('Am I blocked?');

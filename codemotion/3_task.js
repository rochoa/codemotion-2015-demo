var queue = require('queue-async');
var config = require('./config');
var codemotion = require('./../build/Release/codemotion');

var q = queue(config.NUM_TASKS * 2);
for (var i = 0; i < config.NUM_TASKS; i++) {
    q.defer(function(done) {
        done(null, codemotion.simpleTask(config.SECONDS));
    });
}
q.awaitAll(function(err, results) {
    console.dir(results);
});
console.log('Am I blocked?');

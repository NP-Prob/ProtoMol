#include <protomol/analysis/Analysis.h>
#include <protomol/config/Configuration.h>
#include <protomol/topology/GenericTopology.h>
#include <protomol/type/ScalarStructure.h>
#include <protomol/module/MainModule.h>
#include <protomol/ProtoMolApp.h>

using namespace std;
using namespace ProtoMol;
using namespace ProtoMol::Report;

const string Analysis::scope("Analysis");

Analysis::Analysis(bool output) :
	app(0), onOutput(output) {}

void Analysis::initialize(const ProtoMolApp *app) {
	this->app = app;
	doInitialize();
}

bool Analysis::run(long step) {
	doRun(step);
	return true;
}

void Analysis::finalize(long step) {
	doRun(step);
	doFinalize(step);
}

Analysis *Analysis::make(const vector<Value> &values) const {
	assertParameters(values);
	return adjustAlias(doMake(values));
}

bool Analysis::isIdDefined(const Configuration *config) const {
	if( !addDoKeyword()) { return config->valid(getId()); }
	string str("do" + getId());
	if( !config->valid(getId()) || config->empty(str)) { return false; }
	if( !config->valid(str)) { return true; }
	return (bool)( *config )[str];
}

void Analysis::getParameters(vector<Parameter> &parameter) const {
	parameter.push_back(Parameter(getId() + "OnOutput", Value(onOutput), true));
}

bool Analysis:: adjustWithDefaultParameters(std::vector<Value> &values, const Configuration *config) const {
	if( !checkParameterTypes(values)) { return false; }
	return checkParameters(values);
}

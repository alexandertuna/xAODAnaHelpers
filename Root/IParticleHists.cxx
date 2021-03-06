#include <xAODAnaHelpers/IParticleHists.h>
#include <sstream>

ANA_MSG_SOURCE(msgIParticleHists, "IParticleHists")

using std::vector;

IParticleHists :: IParticleHists (std::string name, std::string detailStr, std::string prefix, std::string title) :
  HistogramManager(name, detailStr),
  m_infoSwitch(new HelperClasses::IParticleInfoSwitch(m_detailStr)),
  m_prefix(prefix),m_title(title)
{
  m_debug = false;
}

IParticleHists :: ~IParticleHists () {
  if(m_infoSwitch) delete m_infoSwitch;
}

StatusCode IParticleHists::initialize() {

  // These plots are always made
  m_Pt_l        = book(m_name, m_prefix+"Pt_l",     m_title+" p_{T} [GeV]", 120, 0, 3000.);
  m_Pt          = book(m_name, m_prefix+"Pt",       m_title+" p_{T} [GeV]", 100, 0, 1000.);
  m_Pt_m        = book(m_name, m_prefix+"Pt_m",     m_title+" p_{T} [GeV]", 100, 0,  500.);
  m_Pt_s        = book(m_name, m_prefix+"Pt_s",     m_title+" p_{T} [GeV]", 200, 0,  200.);
  m_Eta         = book(m_name, m_prefix+"Eta",      m_title+" #eta",         80, -4, 4);
  m_Phi         = book(m_name, m_prefix+"Phi",      m_title+" Phi",         120, -TMath::Pi(), TMath::Pi() );
  m_M           = book(m_name, m_prefix+"Mass",     m_title+" Mass [GeV]",  120, 0, 400);
  m_E           = book(m_name, m_prefix+"Energy",   m_title+" Energy [GeV]",120, 0, 4000.);
  m_Rapidity    = book(m_name, m_prefix+"Rapidity", m_title+" Rapidity",    120, -10, 10);

  if(m_debug) Info("IParticleHists::initialize()", m_name.c_str());
  // details of the particle kinematics
  if( m_infoSwitch->m_kinematic ) {
    if(m_debug) Info("IParticleHists::initialize()", "adding kinematic plots");
    m_Px     = book(m_name, m_prefix+"Px",     m_title+" Px [GeV]",     120, 0, 1000);
    m_Py     = book(m_name, m_prefix+"Py",     m_title+" Py [GeV]",     120, 0, 1000);
    m_Pz     = book(m_name, m_prefix+"Pz",     m_title+" Pz [GeV]",     120, 0, 4000);

    m_Et          = book(m_name, m_prefix+"Et",       m_title+" E_{T} [GeV]", 100, 0, 1000.);
    m_Et_m        = book(m_name, m_prefix+"Et_m",     m_title+" E_{T} [GeV]", 100, 0,  500.);
    m_Et_s        = book(m_name, m_prefix+"Et_s",     m_title+" E_{T} [GeV]", 100, 0,  100.);
  }

  // N leading jets
  if( m_infoSwitch->m_numLeading > 0 ){
    std::stringstream pNum;
    std::stringstream pTitle;
    for(int iParticle=0; iParticle < m_infoSwitch->m_numLeading; ++iParticle){
      pNum << iParticle;

      pTitle << iParticle+1;
      switch(iParticle)
	{
	case 0:
	  pTitle << "^{st}";
	  break;
	case 1:
	  pTitle << "^{nd}";
	  break;
	case 2:
	  pTitle << "^{rd}";
	  break;
	default:
	  pTitle << "^{th}";
	  break;
	}

      m_NPt_l.push_back(       book(m_name, (m_prefix+"Pt_l_"+pNum.str()),       pTitle.str()+" "+m_title+" p_{T} [GeV]" ,120,            0,       3000. ) );
      m_NPt .push_back(       book(m_name, (m_prefix+"Pt_"+pNum.str()),       pTitle.str()+" "+m_title+" p_{T} [GeV]" ,100,            0,       1000. ) );
      m_NPt_m.push_back(       book(m_name, (m_prefix+"Pt_m_"+pNum.str()),       pTitle.str()+" "+m_title+" p_{T} [GeV]" ,100,            0,       500. ) );
      m_NPt_s.push_back(       book(m_name, (m_prefix+"Pt_s_"+pNum.str()),       pTitle.str()+" "+m_title+" p_{T} [GeV]" ,100,            0,       100. ) );
      m_NEta.push_back(      book(m_name, (m_prefix+"Eta_"+pNum.str()),      pTitle.str()+" "+m_title+" #eta"        , 80,           -4,           4 ) );
      m_NPhi.push_back(      book(m_name, (m_prefix+"Phi_"+pNum.str()),      pTitle.str()+" "+m_title+" Phi"         ,120, -TMath::Pi(), TMath::Pi() ) );
      m_NM.push_back(        book(m_name, (m_prefix+"Mass_"+pNum.str()),     pTitle.str()+" "+m_title+" Mass [GeV]"  ,120,            0,         400 ) );
      m_NE.push_back(        book(m_name, (m_prefix+"Energy_"+pNum.str()),   pTitle.str()+" "+m_title+" Energy [GeV]",120,            0,       4000. ) );
      m_NRapidity.push_back( book(m_name, (m_prefix+"Rapidity_"+pNum.str()), pTitle.str()+" "+m_title+" Rapidity"    ,120,          -10,          10 ) );
      if(m_infoSwitch->m_kinematic){
	m_NEt .push_back(        book(m_name, (m_prefix+"Et_"+pNum.str()),         pTitle.str()+" "+m_title+" E_{T} [GeV]" ,100,            0,       1000. ) );
	m_NEt_m.push_back(       book(m_name, (m_prefix+"Et_m_"+pNum.str()),       pTitle.str()+" "+m_title+" E_{T} [GeV]" ,100,            0,       500. ) );
	m_NEt_s.push_back(       book(m_name, (m_prefix+"Et_s_"+pNum.str()),       pTitle.str()+" "+m_title+" E_{T} [GeV]" ,100,            0,       100. ) );
      }

      pNum.str("");
      pTitle.str("");

    }//for iParticle
  }

  return StatusCode::SUCCESS;
}

StatusCode IParticleHists::execute( const xAOD::IParticleContainer* particles, float eventWeight, const xAOD::EventInfo* eventInfo) {
  using namespace msgIParticleHists;
  for( auto particle_itr : *particles ) {
    ANA_CHECK( this->execute( particle_itr, eventWeight, eventInfo));
  }

  if( m_infoSwitch->m_numLeading > 0){
    int numParticles = std::min( m_infoSwitch->m_numLeading, (int)particles->size() );
    for(int iParticle=0; iParticle < numParticles; ++iParticle){
      m_NPt_l.at(iParticle)->        Fill( particles->at(iParticle)->pt()/1e3,   eventWeight);
      m_NPt.at(iParticle)->        Fill( particles->at(iParticle)->pt()/1e3,   eventWeight);
      m_NPt_m.at(iParticle)->        Fill( particles->at(iParticle)->pt()/1e3,   eventWeight);
      m_NPt_s.at(iParticle)->        Fill( particles->at(iParticle)->pt()/1e3,   eventWeight);
      m_NEta.at(iParticle)->       Fill( particles->at(iParticle)->eta(),      eventWeight);
      m_NPhi.at(iParticle)->       Fill( particles->at(iParticle)->phi(),      eventWeight);
      m_NM.at(iParticle)->         Fill( particles->at(iParticle)->m()/1e3,    eventWeight);
      m_NE.at(iParticle)->         Fill( particles->at(iParticle)->e()/1e3,    eventWeight);
      m_NRapidity.at(iParticle)->  Fill( particles->at(iParticle)->rapidity(), eventWeight);

      if(m_infoSwitch->m_kinematic){
	float et = particles->at(iParticle)->e()/cosh(particles->at(iParticle)->eta())/1e3;
	m_NEt  .at(iParticle)->        Fill( et,   eventWeight);
	m_NEt_m.at(iParticle)->        Fill( et,   eventWeight);
	m_NEt_s.at(iParticle)->        Fill( et,   eventWeight);
      }

    }
  }

  return StatusCode::SUCCESS;
}

StatusCode IParticleHists::execute( const xAOD::IParticle* particle, float eventWeight, const xAOD::EventInfo* /*eventInfo*/ ) {

  if(m_debug) std::cout << "IParticleHists: in execute " <<std::endl;

  //basic
  m_Pt_l ->      Fill( particle->pt()/1e3,    eventWeight );
  m_Pt ->        Fill( particle->pt()/1e3,    eventWeight );
  m_Pt_m ->      Fill( particle->pt()/1e3,    eventWeight );
  m_Pt_s ->      Fill( particle->pt()/1e3,    eventWeight );
  m_Eta->        Fill( particle->eta(),       eventWeight );
  m_Phi->        Fill( particle->phi(),       eventWeight );
  m_M->          Fill( particle->m()/1e3,     eventWeight );
  m_E->          Fill( particle->e()/1e3,     eventWeight );
  m_Rapidity->   Fill( particle->rapidity(),  eventWeight );

  // kinematic
  if( m_infoSwitch->m_kinematic ) {


    m_Px->  Fill( particle->p4().Px()/1e3,  eventWeight );
    m_Py->  Fill( particle->p4().Py()/1e3,  eventWeight );
    m_Pz->  Fill( particle->p4().Pz()/1e3,  eventWeight );


    m_Et ->        Fill( particle->p4().Et()/1e3,    eventWeight );
    m_Et_m ->      Fill( particle->p4().Et()/1e3,    eventWeight );
    m_Et_s ->      Fill( particle->p4().Et()/1e3,    eventWeight );
  } // fillKinematic

  return StatusCode::SUCCESS;
}




StatusCode IParticleHists::execute( const xAH::Particle* particle, float eventWeight, const xAH::EventInfo* /*eventInfo*/ ) {

  if(m_debug) std::cout << "IParticleHists: in execute " <<std::endl;

  const TLorentzVector& partP4 = particle->p4;

  //basic
  m_Pt_l ->      Fill( partP4.Pt(),    eventWeight );
  m_Pt ->        Fill( partP4.Pt(),    eventWeight );
  m_Pt_m ->      Fill( partP4.Pt(),    eventWeight );
  m_Pt_s ->      Fill( partP4.Pt(),    eventWeight );
  m_Eta->        Fill( partP4.Eta(),       eventWeight );
  m_Phi->        Fill( partP4.Phi(),       eventWeight );
  m_M->          Fill( partP4.M(),     eventWeight );
  m_E->          Fill( partP4.E(),     eventWeight );
  m_Rapidity->   Fill( partP4.Rapidity(),  eventWeight );

  // kinematic
  if( m_infoSwitch->m_kinematic ) {
    m_Px->  Fill( partP4.Px(),  eventWeight );
    m_Py->  Fill( partP4.Py(),  eventWeight );
    m_Pz->  Fill( partP4.Pz(),  eventWeight );

    m_Et ->        Fill( partP4.Et(),    eventWeight );
    m_Et_m ->      Fill( partP4.Et(),    eventWeight );
    m_Et_s ->      Fill( partP4.Et(),    eventWeight );
  } // fillKinematic

  return StatusCode::SUCCESS;
}


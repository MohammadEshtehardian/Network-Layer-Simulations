// This is base structure of the code needed for your network layer assignment. Obviously the code is not complete and you have to do the rest. Please pay attention to the comments.

// Initialization
// At first you have to include the modules you need as follows:
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/applications-module.h"
#include "ns3/animation-interface.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/mobility-model.h"
#include "ns3/mobility-helper.h"
#include "ns3/constant-position-mobility-model.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace ns3;
int main (int argc, char *argv[])
{
	// Initialization
	// The next line enables you showing some messages while running the code:
	
	Time::SetResolution (Time::NS);
	NS_LOG_COMPONENT_DEFINE ("Network_Layer_Assignment");
	LogComponentEnable ("Network_Layer_Assignment", LOG_LEVEL_INFO);
	
	// At the beginning of the main function, you have to configure some parameters as follows:
	
	uint32_t PacketSize = 512; // bytes
	std::string DataRate ("1Mbps");
	uint16_t num_Nodes = 9;
	uint16_t UDPport = 9;
	bool tracing = false;
	
	// While using NS3, you can use "CommandLine" object to override any of the default parameters, including the above parameters, at run-time:
	
	CommandLine cmd;
	cmd.AddValue ("PacketSize", "size of application packet sent", PacketSize);
	cmd.AddValue ("DataRate", "rate of pacekts sent", DataRate);
	cmd.AddValue ("tracing", "turn on ascii and pcap tracing", tracing);
	cmd.Parse (argc, argv);
	
	// The following lines override the default values of the OnOffApplication used in data generation:
	
	Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue(PacketSize));
	Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue (DataRate));
	Config::SetDefault ("ns3::Ipv4GlobalRouting::RespondToInterfaceEvents",BooleanValue(true));
	
	// Finally, you should enable Packet Meta data for animator and the name of file for animation output:
	
	ns3::PacketMetadata::Enable();
	std::string animFile = "Network_Layer_Assignment_Animation.xml" ;

	// Network Topology
	// In the next step, you have to create the network topology. Network nodes are stored in a container class called "NodeContainer":
	
	NodeContainer nodes;
	nodes.Create (num_Nodes);

	// Add Mobility Model
	for (uint8_t i = 0; i < num_Nodes; ++i)
    {
       nodes.Get (i)->AggregateObject (CreateObject<ConstantPositionMobilityModel> ());
    }
	
	// Create Links
	// Now, you should group nodes to make links between them.
	NodeContainer AB = NodeContainer (nodes.Get(0) , nodes.Get(1));
	NodeContainer AH = NodeContainer (nodes.Get(0) , nodes.Get(7));
	NodeContainer BC = NodeContainer (nodes.Get(1) , nodes.Get(2));
	NodeContainer BH = NodeContainer (nodes.Get(1) , nodes.Get(7));
	NodeContainer BI = NodeContainer (nodes.Get(1) , nodes.Get(8));
	NodeContainer CD = NodeContainer (nodes.Get(2) , nodes.Get(3));
	NodeContainer CI = NodeContainer (nodes.Get(2) , nodes.Get(8));
	NodeContainer DE = NodeContainer (nodes.Get(3) , nodes.Get(4));
	NodeContainer DF = NodeContainer (nodes.Get(3) , nodes.Get(5));
	NodeContainer DI = NodeContainer (nodes.Get(3) , nodes.Get(8));
	NodeContainer EF = NodeContainer (nodes.Get(4) , nodes.Get(5));
	NodeContainer FG = NodeContainer (nodes.Get(5) , nodes.Get(6));
	NodeContainer FI = NodeContainer (nodes.Get(5) , nodes.Get(8));
	NodeContainer GH = NodeContainer (nodes.Get(6) , nodes.Get(7));
	NodeContainer GI = NodeContainer (nodes.Get(6) , nodes.Get(8));
	NodeContainer HI = NodeContainer (nodes.Get(7) , nodes.Get(8));
	
	// Now you should assign bandwidth and delay to each link.
	
	PointToPointHelper p2p;
	p2p.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
	p2p.SetChannelAttribute ("Delay", StringValue ("10ms"));
	
	// Install p2p on each link
	// Now you have to install p2p on each link.
	NetDeviceContainer dAB = p2p.Install (AB);
	NetDeviceContainer dAH = p2p.Install (AH);
	NetDeviceContainer dBC = p2p.Install (BC);
	NetDeviceContainer dBH = p2p.Install (BH);
	NetDeviceContainer dBI = p2p.Install (BI);
	NetDeviceContainer dCD = p2p.Install (CD);
	NetDeviceContainer dCI = p2p.Install (CI);
	NetDeviceContainer dDE = p2p.Install (DE);
	NetDeviceContainer dDF = p2p.Install (DF);
	NetDeviceContainer dDI = p2p.Install (DI);
	NetDeviceContainer dEF = p2p.Install (EF);
	NetDeviceContainer dFG = p2p.Install (FG);
	NetDeviceContainer dFI = p2p.Install (FI);
	NetDeviceContainer dGH = p2p.Install (GH);
	NetDeviceContainer dGI = p2p.Install (GI);
	NetDeviceContainer dHI = p2p.Install (HI);

	// Network Layer Parameters
	// In this part, you will configure the network parameters. First, you have to install IPv4 on the network via using InternetStackHelper. This helper uses static routing as its first priority algorithm for routing; therefore, we redefine the priority: ####

	NS_LOG_INFO("Setting routing protocols");
	Ipv4StaticRoutingHelper staticRouting;
	Ipv4GlobalRoutingHelper globalRouting;
	Ipv4ListRoutingHelper list;
	list.Add(staticRouting,0);
	list.Add(globalRouting,10);
	
	// Install network stacks on the nodes
	InternetStackHelper internet;
	internet.SetRoutingHelper(list);
	internet.Install(nodes);
	
	// Now, you should assign IP to each node for routing and also assign the metric of each link to it.
	Ipv4AddressHelper ipv4;
	ipv4.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer iAB = ipv4.Assign (dAB);
	iAB.SetMetric(0,29);
	iAB.SetMetric(1,29);
	Ipv4InterfaceContainer iAH = ipv4.Assign (dAH);
	iAH.SetMetric(0,25);
	iAH.SetMetric(1,25);
	Ipv4InterfaceContainer iBC = ipv4.Assign (dBC);
	iBC.SetMetric(0,26);
	iBC.SetMetric(1,26);
	Ipv4InterfaceContainer iBH = ipv4.Assign (dBH);
	iBH.SetMetric(0,30);
	iBH.SetMetric(1,30);
	Ipv4InterfaceContainer iBI = ipv4.Assign (dBI);
	iBI.SetMetric(0,24);
	iBI.SetMetric(1,24);
	Ipv4InterfaceContainer iCD = ipv4.Assign (dCD);
	iCD.SetMetric(0,29);
	iCD.SetMetric(1,29);
	Ipv4InterfaceContainer iCI = ipv4.Assign (dCI);
	iCI.SetMetric(0,28);
	iCI.SetMetric(1,28);
	Ipv4InterfaceContainer iDE = ipv4.Assign (dDE);
	iDE.SetMetric(0,25);
	iDE.SetMetric(1,25);
	Ipv4InterfaceContainer iDF = ipv4.Assign (dDF);
	iDF.SetMetric(0,24);
	iDF.SetMetric(1,24);
	Ipv4InterfaceContainer iDI = ipv4.Assign (dDI);
	iDI.SetMetric(0,27);
	iDI.SetMetric(1,27);
	Ipv4InterfaceContainer iEF = ipv4.Assign (dEF);
	iEF.SetMetric(0,26);
	iEF.SetMetric(1,26);
	Ipv4InterfaceContainer iFG = ipv4.Assign (dFG);
	iFG.SetMetric(0,25);
	iFG.SetMetric(1,25);
	Ipv4InterfaceContainer iFI = ipv4.Assign (dFI);
	iFI.SetMetric(0,25);
	iFI.SetMetric(1,25);
	Ipv4InterfaceContainer iGH = ipv4.Assign (dGH);
	iGH.SetMetric(0,28);
	iGH.SetMetric(1,28);
	Ipv4InterfaceContainer iGI = ipv4.Assign (dGI);
	iGI.SetMetric(0,24);
	iGI.SetMetric(1,24);
	Ipv4InterfaceContainer iHI = ipv4.Assign (dHI);
	iHI.SetMetric(0,26);
	iHI.SetMetric(1,26);
	
	// Finally, you have to add the following code to initialize routing database and set up the routing tables in the nodes:
	
	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

	// Building Flows
	// After doing the preceding part, now it is time to determine the application with which the nodes will function. You should install UDP application on node "A" and "E". First, use the following code:

	PacketSinkHelper UDPsink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), UDPport));
	ApplicationContainer App;
	NodeContainer SourceNode = NodeContainer (nodes.Get (0));
	NodeContainer SinkNode = NodeContainer (nodes.Get (4)); // A = 0, B = 1, C = 2, D = 3, E = 4, ...
	
	// To Create a UDP packet sink to receive these packets use the following code:
	
	App = UDPsink.Install (SinkNode);
	App.Start (Seconds (0.0));
	App.Stop (Seconds (10.0));
	Address E_Address(InetSocketAddress(iEF.GetAddress (0), UDPport));
	
	// To Create a UDP packet source to send these packets use the following code:
	
	OnOffHelper UDPsource ("ns3::UdpSocketFactory", E_Address);
	UDPsource.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
	UDPsource.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
	UDPsource.Install(SourceNode);
	App.Start (Seconds (1.0));
	App.Stop (Seconds (10.0));

	
	// Analysis
	// You should determine your nodes places for NetAnim. Do it with the following code:
	AnimationInterface anim (animFile);
	Ptr <Node> n = nodes.Get (0);
	anim.SetConstantPosition (n, 0, 20);
	n = nodes.Get(1);
	anim.SetConstantPosition(n, 10, 10);
	n = nodes.Get(2);
	anim.SetConstantPosition(n, 20, 0);
	n = nodes.Get(3);
	anim.SetConstantPosition(n, 30, 10);
	n = nodes.Get(4);
	anim.SetConstantPosition(n, 40, 20);
	n = nodes.Get(5);
	anim.SetConstantPosition(n, 30, 30);
	n = nodes.Get(6);
	anim.SetConstantPosition(n, 20, 40);
	n = nodes.Get(7);
	anim.SetConstantPosition(n, 10, 30);
	n = nodes.Get(8);
	anim.SetConstantPosition(n, 20, 20);
	
	// You can use the following code to generate the xml file used in netAnim.
	if (tracing == true)
	{
	AsciiTraceHelper ascii;
	p2p.EnableAsciiAll (ascii.CreateFileStream ("Network_Layer_Assignment.tr"));
	p2p.EnablePcapAll ("Network_Layer_Assignment");
	}
	
	// To print routing tables you have to add the following code:
	NS_LOG_INFO("Printing routing table before setting down.");
	Ptr <OutputStreamWrapper> stream1 = Create<OutputStreamWrapper> ("Routing_Table_before_SetDown", std::ios::out);
	Ipv4GlobalRoutingHelper helper2;
	helper2.PrintRoutingTableAllAt(Seconds(2.0),stream1);

	// This part can be used to shutdown links
	Ptr<Node> node1=nodes.Get(8);
	Ptr<Ipv4> ipv41=node1->GetObject<Ipv4>();
	Simulator::Schedule(Seconds(3),&Ipv4::SetDown,ipv41,1);
	Simulator::Schedule(Seconds(3),&Ipv4::SetDown,ipv41,2);
	Simulator::Schedule(Seconds(3),&Ipv4::SetDown,ipv41,3);
	Simulator::Schedule(Seconds(3),&Ipv4::SetDown,ipv41,4);
	Simulator::Schedule(Seconds(3),&Ipv4::SetDown,ipv41,5);
	Simulator::Schedule(Seconds(3),&Ipv4::SetDown,ipv41,6);
	
	// Printing the routing table after SetDown
	NS_LOG_INFO("Printing routing table after setting down.");
	Ptr<OutputStreamWrapper> stream2 = Create<OutputStreamWrapper> ("Routing_Table_after_SetDown", std::ios::out);
	helper2.PrintRoutingTableAllAt(Seconds(4.0),stream2);
	
	// You should just add the following two lines to your code before running it:
	NS_LOG_INFO("Run simulation");
	Simulator::Stop(Seconds(10.0));
	Simulator::Run();
	
	// After everything is done, you have to destroy the simulation using the following command:
	Simulator::Destroy();
	NS_LOG_INFO("Done");

	
	return 0;

}

//
//  DBmanger.swift
//  HeartRateExporter
//
//  Created by  on 4/20/17.
//  Copyright © 2017 Sunset Lake Software LLC. All rights reserved.
//

import Foundation
import UIKit
import AWSDynamoDB
import AWSCore
import AWSCognito
import AWSSQS
import AWSSNS
import AWSS3
class Heartratemonitor: AWSDynamoDBObjectModel, AWSDynamoDBModeling {
    
    var Heartrate: String?
    var Date: String?
    var Time: String?
    
    class func dynamoDBTableName() -> String {
        
        return "ioshealthratemonitor-mobilehub-604855940-heartratemonitor"
    }
    
    class func hashKeyAttribute() -> String {
        
        return "Heartrate"
    }
    
    
}
